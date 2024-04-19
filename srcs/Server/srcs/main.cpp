#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_CLIENTS 1024
#define BUFFER_SIZE 42 * 4096

typedef struct s_Client{
    int id, fd;
}   t_Client;

t_Client clients[MAX_CLIENTS];
int next_id = 0;

size_t ft_strlen(const char *s)
{
    size_t i = 0;
    while (s[i])
        i++;
    return i;
}

void fatal_error()
{
    write(2, "Fatal error\n", ft_strlen("Fatal error\n"));
    exit(1);
}

void send_all(char *buff);

int main(int ac, char **av)
{
    if (ac != 2)
    {
        write(2, "Wrong number of arguments\n", ft_strlen("Wrong number of arguments\n"));
        exit(1);
    }

    int port_num = atoi(av[1]);
    int serv_fd, new_fd, len;
	struct sockaddr_in servaddr, cli;
    // socket create and verification 

	serv_fd = socket(AF_INET, SOCK_STREAM, 0); 
	if (serv_fd == -1) { 
		fatal_error();
	} 
	
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(2130706433); //127.0.0.1
	servaddr.sin_port = htons(port_num);
  
	// Binding newly created socket to given IP and verification 
	if ((bind(serv_fd, (const struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
		fatal_error();
	} 
	
	if (listen(serv_fd, 10) != 0) {
		fatal_error();
	}
    
    fd_set read_fds;
    int max_fd;
    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(serv_fd, &read_fds);
        max_fd = serv_fd;

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients[i].fd > 0)
            {
                FD_SET(clients[i].fd, &read_fds);
                if (clients[i].fd > max_fd)
                    max_fd = clients[i].fd;
            }
        }

        int activate = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activate < 0)
            fatal_error();

        if (FD_ISSET(serv_fd, &read_fds))
        {
            len = sizeof(cli);
            new_fd = accept(serv_fd, (struct sockaddr *)&cli, (socklen_t *)&len);
            if (new_fd < 0)
                fatal_error();
            for (int i = 0; i < MAX_CLIENTS; i++)
            {
                if (clients[i].fd == 0)
                {
                    char buff[BUFFER_SIZE];
                    sprintf(buff, "server: client %d just arrived\n", next_id);
                    printf("server: client %d just arrived\n", next_id);
                    send_all(buff);
                    clients[i].fd = new_fd;
                    clients[i].id = next_id++;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            t_Client client = clients[i];
            if (FD_ISSET(client.fd, &read_fds))
            {
                char buff[BUFFER_SIZE];
                int read_bytes = recv(client.fd, buff, BUFFER_SIZE, 0);
                if (read_bytes <= 0) // read_bytes <= 0  (4.16 틀린 이유)
                {
                    sprintf(buff, "server: client %d just left\n", client.id);
                    printf("server: client %d just left\n", client.id);
                    close(client.fd);
                    clients[i].fd = 0;
                    send_all(buff);
                }
                // else if (read_bytes < 0) //이거 빼야함 (4.16 틀린 이유)
                //     fatal_error();
                else
                {
                    buff[read_bytes] = 0;
                    char new_buff[BUFFER_SIZE];
                    
                    for (int i = 0, j = 0; i < read_bytes; i++, j++)
                    {
                        new_buff[j] = buff[i];
                        if (buff[i] == '\n')
                        {
                            new_buff[j] = 0;
                            char nn_buff[BUFFER_SIZE];
                            sprintf(nn_buff, "client %d: %s", client.id, new_buff);
                            printf("client %d: %s", client.id, new_buff);
                            send_all(nn_buff);
                            j = -1;
                        }
                    }
                }
            
            }
        }
    }
}

void send_all(char *buff)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].fd > 0)
            send(clients[i].fd, buff, ft_strlen(buff), 0);
    }
}
