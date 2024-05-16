#include <cstdlib>
#include <cstring>
#include <sstream>

#include "Server.hpp"
#include "Util.hpp"
#include "Thread.hpp"
#include "GameFrame.hpp"
#include "Cache.hpp"
#include "MatchMaking.hpp"

// void playGame();

static int extract_message(char **buf, char **msg)
{
    char *newbuf;
    int i;

    *msg = 0;
    if (*buf == 0)
        return (0);
    i = 0;
    while ((*buf)[i])
    {
        if ((*buf)[i] == '\n')
        {
            newbuf = (char *)calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
            if (newbuf == 0)
                error_msg(FATAL_ERR);
            strcpy(newbuf, *buf + i + 1);
            *msg = *buf;
            (*msg)[i + 1] = 0;
            *buf = newbuf;
            return (1);
        }
        i++;
    }
    return (0);
}

static char *str_join(char *buf, char *add)
{
    char *newbuf;
    int len;

    if (buf == 0)
        len = 0;
    else
        len = strlen(buf);
    newbuf = (char *)malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
    if (newbuf == 0)
        error_msg(FATAL_ERR);
    newbuf[0] = 0;
    if (buf != 0)
    {
        strcat(newbuf, buf);
        free(buf);
    }
    strcat(newbuf, add);
    return (newbuf);
}

void Server::bindAndListen()
{
    struct sockaddr_in servaddr;

    serv_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (serv_fd == -1)
        error_msg(FATAL_ERR);

    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = _ip_address;
    servaddr.sin_port = htons(_port_num);

    // Binding newly created socket to given IP and verification
    if ((bind(serv_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0)
        error_msg(FATAL_ERR);

    if (listen(serv_fd, 10) != 0)
        error_msg(FATAL_ERR);
}

static void processAccordingToErrno()
{
    switch (errno)
    {
    case EBADF: // 파일 디스크립터가 유효하지 않은 경우
        std::cerr << "[select] failed due to invalid file descriptor\n";
        break;
    case EINTR: // select() 함수가 인터럽트에 의해 중단된 경우
        std::cerr << "[select] was interrupted\n";
        break;
    case EINVAL: // timeout 파라미터가 잘못된 경우
        std::cerr << "[select] failed due to invalid timeout\n";
        break;
    default:
        std::cerr << "[select] failed\n";
        error_msg(FATAL_ERR);
    }
}

void Server::prepareReadFds(fd_set &read_fds, int &max_fd)
{
    FD_ZERO(&read_fds);
    FD_SET(serv_fd, &read_fds);

    max_fd = serv_fd;

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].fd > 0)// && clients[i].is_game_ing == false)
        {
            FD_SET(clients[i].fd, &read_fds);
            if (clients[i].fd > max_fd)
                max_fd = clients[i].fd;
        }
    }
}

void Server::acceptNewClient(struct sockaddr_in &cli)
{
    int len = sizeof(cli);
    int new_fd = accept(serv_fd, (struct sockaddr *)&cli, (socklen_t *)&len);
    if (new_fd < 0)
        error_msg(FATAL_ERR);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        s_Client &client = clients[i];
        if (client.fd == 0)
        {
            client.fd = new_fd;
            client.id = next_id++;
            client.buff = nullptr;
            client.waiting_game = 0;
            client.is_game_ing = false;
            sendClientMessage(client.id, ARRIVE, nullptr);
            break;
        }
    }
}

void Server::handleClientMessage(fd_set &read_fds)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        s_Client &client = clients[i];
        if ((client.fd > 0 && FD_ISSET(client.fd, &read_fds)) && client.is_game_ing == false)
        {
            char buff[BUFFER_SIZE];
            int read_bytes = recv(client.fd, buff, BUFFER_SIZE - 1, 0);
            if (read_bytes <= 0)
            {
                std::cout << "Client[" << client.id << "] left" << std::endl;
                close(client.fd);
                client.fd = 0;
                sendClientMessage(client.id, LEFT, NULL);
            }
            else
            {
                buff[read_bytes] = 0;
                std::cout << "test: " << client.id << " :" << buff; // test
                client.buff = str_join(client.buff, buff);
                while (read_bytes == BUFFER_SIZE - 1)
                {
                    read_bytes = recv(client.fd, buff, BUFFER_SIZE - 1, 0);
                    if (read_bytes <= 0)
                        break;
                    buff[read_bytes] = 0;
                    client.buff = str_join(client.buff, buff);
                }
                char *msg;
                while (extract_message(&client.buff, &msg) > 0)
                {
                    std::string str(msg);
                    if (str.compare("Game Start\n") == 0)
                    {
                        client.waiting_game = 1;
                        MatchMaking::addPlayer(&client);
                    }
                    else
                        sendClientMessage(client.id, MSG, msg);
                    free(msg);
                }
            }
        }
    }
}

void Server::handleClientConnections()
{
    fd_set read_fds;
    struct sockaddr_in cli;
    while (1)
    {
        int max_fd = serv_fd;
        prepareReadFds(read_fds, max_fd);

        int rv = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
        if (rv < 0)
        {
            processAccordingToErrno();
            continue;
        }
        if (rv == 0) // timeout
            continue;

        if (FD_ISSET(serv_fd, &read_fds))
        {
            acceptNewClient(cli);
        }

        handleClientMessage(read_fds);
    }
}

void Server::runServer()
{
    bindAndListen();
    handleClientConnections();
}

void Server::sendClientMessage(int my_id, enum e_msg flag, char *msg)
{
    char buff[BUFFER_SIZE];
    int read_bytes;
    if (flag == ARRIVE)
    {
        read_bytes = sprintf(buff, "server: client %d just arrived\n", my_id);
    }
    else if (flag == LEFT)
    {
        read_bytes = sprintf(buff, "server: client %d just left\n", my_id);
    }
    else if (flag == MSG)
    {
        read_bytes = sprintf(buff, "client %d: ", my_id);
    }

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        s_Client &client = clients[i];
        if (client.fd > 0 && client.id != my_id)
        {
            int bytes_sent = send(client.fd, buff, read_bytes, 0);
            if (bytes_sent == -1)
            {
                std::cerr << "Failed to send message to client " << client.id << std::endl;
                continue;
            }
            if (flag == MSG)
            {
                bytes_sent = send(client.fd, msg, strlen(msg), 0);
                if (bytes_sent == -1)
                {
                    std::cerr << "Failed to send message to client " << client.id << std::endl;
                }
            }
        }
    }
    // test
    std::cout << buff;
    if (flag == MSG)
        std::cout << msg;
}

void Server::sendGameData(e_game flag, int *players_id, GameData *data)
{
    for (int i = 0; i < 2; i++)
    {
        s_Client &client = clients[players_id[i]];

        ssize_t bytes_sent;
        std::string msg;

        switch (flag)
        {
        case GAME_START:
            msg = "Game Start";
            bytes_sent = send(client.fd, msg.c_str(), msg.size(), 0);
            break;
        case GAME_LOADING:
            msg = "Loading...";
            bytes_sent = send(client.fd, msg.c_str(), msg.size(), 0);
            break;
        case GAME_ING:
            bytes_sent = send(client.fd, data, sizeof(GameData), 0);
            // std::cout << "sendGameData" << std::endl;
            break;
        default:
            bytes_sent = send(client.fd, data, sizeof(GameData), 0);
            msg = "Game End";
            bytes_sent = send(client.fd, msg.c_str(), msg.size(), 0);

            std::stringstream ss;
            ss << "[sendGameData] Game End client[" << client.id << "]\n";
            std::cout << ss.str();

            break;
        }

        if (bytes_sent == -1)
        {
            std::stringstream ss;
            ss << "[sendGameData] Disconnected client[" << client.id << "]\n";
            std::cerr << ss.str();
            Cache::atom_stop = true;
            return;
        }
    }
}

int Server::receiveGameData(s_Client &player, int &isGameStart)
{
    char buffer[BUFFER_SIZE] = {0};
    std::stringstream ss;

    fd_set set;
    struct timeval timeout;
    int rv;
    FD_ZERO(&set);           /* clear the set */
    FD_SET(player.fd, &set); /* add our file descriptor to the set */
    ssize_t valread = 0;

    timeout.tv_sec = 0;
    timeout.tv_usec = 500000; // 0.5 seconds

    rv = select(player.fd + 1, &set, NULL, NULL, &timeout);

    if (rv == -1)
        perror("select"); /* an error accured */
    else if (rv == 0)
        return EXIT_FAILURE; /* a timeout occured */
    else
        valread = recv(player.fd, buffer, BUFFER_SIZE, 0);

    if (valread <= 0)
    {
        ss << "[receiveGameData] Failed to receive data from player[" << player.id << "]\n";
        std::cerr << ss.str();
        return EXIT_FAILURE;
    }

    if (isGameStart == GAME_END)
        return EXIT_FAILURE;

    buffer[valread] = 0;
    player.msg = buffer;
    // ss << "player[" << player.id << "]: " << player.msg;
    // std::cout << ss.str() << std::endl; // test
    return EXIT_SUCCESS;
}