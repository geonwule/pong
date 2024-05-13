#include <cstdlib>
#include <cstring>
#include <sstream>

#include "Server.hpp"
#include "Util.hpp"
#include "Thread.hpp"
#include "GameFrame.hpp"
#include "Cache.hpp"

void playGame();

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

void Server::handleClientConnections()
{
    fd_set read_fds;
    struct sockaddr_in cli;
    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(serv_fd, &read_fds);

        int max_fd = serv_fd;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients[i].fd > 0 && clients[i].game_ing == 0)
            {
                FD_SET(clients[i].fd, &read_fds);
                if (clients[i].fd > max_fd)
                    max_fd = clients[i].fd;
            }
        }

        int rv = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
        if (rv < 0)
            error_msg(FATAL_ERR);
        if (rv == 0) // timeout
            continue;

        if (FD_ISSET(serv_fd, &read_fds))
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
                    client_num++;
                    sendClientMessage(client.id, ARRIVE, nullptr);
                    if (client_num == 2)
                    {
                        // thread_arr[0] = new std::thread(playGame);
                        Cache::atom_stop = false;
                        Thread::createThread(playGame);
                        client_num = 0;
                    }
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            s_Client &client = clients[i];
            if (client.fd > 0 && FD_ISSET(client.fd, &read_fds) && client.game_ing == 0)
            {
                char buff[BUFFER_SIZE];
                int read_bytes = recv(client.fd, buff, BUFFER_SIZE - 1, 0);
                if (read_bytes <= 0)
                {
                    std::cout << "Client[" << client.id << "] left" << std::endl;
                    close(client.fd);
                    clients[i].fd = 0;
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
                    while (extract_message(&clients[i].buff, &msg) > 0)
                    {
                        sendClientMessage(client.id, MSG, msg);
                        free(msg);
                    }
                }
            }
        }
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

void Server::sendGameData(e_game flag, GameData *data)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        s_Client &client = clients[i];
        if (client.fd > 0)
        {
            ssize_t bytes_sent;
            if (flag == GAME_START)
            {
                std::string msg("Game Start");
                bytes_sent = send(client.fd, msg.c_str(), msg.size(), 0);
            }
            else if (flag == GAME_LOADING)
            {
                std::string msg("Loading...");
                bytes_sent = send(client.fd, msg.c_str(), msg.size(), 0);
            }
            else if (flag == GAME_ING)
            {
                bytes_sent = send(client.fd, data, sizeof(GameData), 0);
                // std::cout << "sendGameData" << std::endl;
            }
            else
            {
                std::string msg("Game End");
                bytes_sent = send(client.fd, msg.c_str(), msg.size(), 0);
            }

            if (bytes_sent == -1)
            {
                std::stringstream ss;
                ss << "Disconnected client[" << client.id << "]";
                std::cerr << ss.str() << std::endl;
                Cache::atom_stop = true;
                return ;
            }
        }
    }
}

void Server::receiveGameData(s_Client &player)
{
    char buffer[BUFFER_SIZE] = {0};
    std::stringstream ss;

    ssize_t valread = recv(player.fd, buffer, BUFFER_SIZE, 0);
    if (valread <= 0)
    {
        ss << "Failed to receive data from player[" << player.id << "]";
        std::cerr << ss.str() << std::endl;
        return;
    }
    buffer[valread] = 0;
    player.msg = buffer;
    ss << "player[" << player.id << "]: " << player.msg;
    std::cout << ss.str() << std::endl; // test
}