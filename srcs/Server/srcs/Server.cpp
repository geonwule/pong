#include "Server.hpp"

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
                    send_all(client.id, ARRIVE, nullptr, clients);
                    // if (client_num == 2)
                    // {
                    //     thread_arr[0] = new std::thread(playGame);
                    //     client_num = 0;
                    // }
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
                    send_all(client.id, LEFT, NULL, clients);
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
                        send_all(client.id, MSG, msg, clients);
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