#include "Server.hpp"
#include "GameFrame.hpp"
#include "Thread.hpp"

extern std::atomic<bool> atom_stop;
void playGame();

Server *Server::_instance = nullptr;

Server::Server(const char *port_num) : _ip_address(inet_addr(IP_ADDRESS)), _port_num(atoi(port_num))
{
    if (_port_num < 0 || _port_num > 65535)
    {
        std::cerr << "port_num is invalid" << std::endl;
        error_msg(FATAL_ERR);
    }
}

Server::Server(const char *ip_address, const char *port_num) : _ip_address(inet_addr(ip_address)), _port_num(atoi(port_num))
{
    if (_port_num < 0 || _port_num > 65535)
    {
        std::cerr << "port_num is invalid" << std::endl;
        error_msg(FATAL_ERR);
    }
}

Server::~Server()
{
    close(serv_fd);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].fd > 0)
            close(clients[i].fd);
    }
}

int Server::setInstance(const char *port_num)
{
    if (_instance == nullptr)
    {
        _instance = new Server(port_num);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int Server::setInstance(const char *ip, const char *port_num)
{
    if (_instance == nullptr)
    {
        _instance = new Server(ip, port_num);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

Server *Server::getInstance()
{
    return _instance;
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
    while(1)
    {
        FD_ZERO(&read_fds);
        FD_SET(serv_fd, &read_fds);

        int max_fd = serv_fd;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients[i].fd > 0)
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
                    sendAll(client.id, ARRIVE, nullptr);
                    if (client_num == 2)
                    {
                        // thread_arr[0] = new std::thread(playGame);
                        atom_stop = false;
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
            if (client.fd > 0 && FD_ISSET(client.fd, &read_fds))
            {
                char buff[BUFFER_SIZE];
                int read_bytes = recv(client.fd, buff, BUFFER_SIZE - 1, 0);
                if (read_bytes <= 0)
                {
                    std::cout << "Client[" << client.id << "] left" << std::endl;
                    close(client.fd);
                    clients[i].fd = 0;
                    sendAll(client.id, LEFT, NULL);
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
                        sendAll(client.id, MSG, msg);
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

void Server::sendAll(int my_id, enum e_msg flag, char *msg)
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

void Server::sendGameData(const std::string &msg, GameData &data)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        s_Client &client = clients[i];
        if (client.fd > 0)
        {
            ssize_t bytes_sent;
            if (msg == "")
            {
                bytes_sent = send(client.fd, &data, sizeof(data), 0);
                std::cout << "sendGameData" << std::endl;
            }
            else
                bytes_sent = send(client.fd, msg.c_str(), msg.size(), 0);
            if (bytes_sent == -1)
            {
                std::cerr << "Disconnected client[" << client.id << "]" << std::endl;
                // atom_stop = true;
                // return ;
            }
        }
    }
}