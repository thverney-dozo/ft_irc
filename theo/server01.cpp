#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <iostream>
#include <fcntl.h>
#include <vector>
#include "Client.hpp"

void 	error_handling(std::string);
int     setup_server(int ac, char **av);
void    *handle_connection(int serv_sock, std::vector<Client*> *clients);
int     accept_connection(int serv_socket, std::vector<Client*> *clients);
int		ft_strlen(const char *str);

int 	main(int argc, char **argv)
{
	int serv_sock;
	// std::vector<Client*> *clients = new std::vector<Client*>;
	std::vector<Client*> clients;
	
	serv_sock = setup_server(argc, argv);
	handle_connection(serv_sock, &clients);
	close(serv_sock);

	return 0;
}

int     setup_server(int ac, char **argv)
{
	int serv_sock;
	struct sockaddr_in serv_adr;

	if (ac != 2){
		std::cout << "Usage: " << argv[0] << " <port>"<< std::endl;
		exit(1);
	}
	serv_sock = socket(AF_INET, SOCK_STREAM, 0); 	// Create server's FD
	if (serv_sock < 0) 
		error_handling("ERROR opening socket");
		int enable = 1;
	if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
		error_handling("setsockopt(SO_REUSEADDR) failed");
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET; 					// Internet address
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);	// Specify that server will respond to any address
	serv_adr.sin_port = htons(atoi(argv[1])); 		// Server will listen to this port
	
	if (bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");
	return (serv_sock);
}

void    *handle_connection(int serv_sock, std::vector<Client*> *clients)
{
	struct timeval timeout;
	fd_set reads, cpy_reads;
	int fd_max, fd_num;
	char buf[1024];
	int client_nb = 0;
	/*Set the fd set*/
	FD_ZERO(&reads);
	FD_SET(serv_sock, &reads);
	fd_max = serv_sock;

	while (1)
	{
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;
		if((fd_num = select(fd_max + 1, &cpy_reads, 0 , 0, &timeout)) == -1)
			break;
		if (fd_num == 0)
			continue;
		for (int i = 0; i < fd_max + 1; i++)
		{
			fcntl(i, F_SETFL, O_NONBLOCK);
			if (FD_ISSET(i, &cpy_reads))
			{
				if (i == serv_sock)
				{
					int clnt_sock = accept_connection(serv_sock, clients);
					FD_SET(clnt_sock, &reads);
					write(clnt_sock, "Bonjour, bienvenue.\n", strlen("Bonjour, bienvenue.\n"));
					std::cout << "[Client connected]" << std::endl;
					write(clnt_sock, "Enter Nickname: ", strlen("Enter Nickname: "));
					if (fd_max < clnt_sock)
						fd_max = clnt_sock;
				}
				else
				{
					int str_len;
					if ((str_len = read(i, buf, 1024)) == 0)
					{
						FD_CLR(i, &reads);
						close(i);
						std::cout << "[Client disconnected] (";
						std::vector<Client*>::iterator ite = clients->end();
						for (std::vector<Client*>::iterator it = clients->begin(); it != ite; ++it) // find my client
						{
							if (i == (*it)->getFd()) {
								std::cout << (*it)->getName() << ")" << std::endl;
								clients->erase(it);
								break;
							}
						}
					}
					else
					{
						std::vector<Client*>::iterator ite = clients->end();
						for (std::vector<Client*>::iterator it = clients->begin(); it != ite; ++it) // find my client
						{
							if (i == (*it)->getFd()) {
								if((*it)->getStat() == false) {  // has my client named himself
									std::string new_name(buf);
									(*it)->setName(new_name);
									(*it)->setStat(true);
									new_name.clear();
									new_name = (*it)->getName();
									write(i, "Welcome ", strlen("Welcome "));
									write(i, new_name.c_str(), ft_strlen(new_name.c_str()));
									write(i, "\n", 1);
								}
								break;
							}
						}
					}
					memset(buf, 0, 1024);
				}
			}
		}
	}
	return NULL;
}

int     accept_connection(int serv_sock, std::vector<Client*> *clients)
{
	struct sockaddr_in clnt_adr;
	socklen_t adr_sz = sizeof(clnt_adr);
	int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);

    Client *new_client = new Client(clnt_sock, clnt_adr, adr_sz);
	clients->push_back(new_client);

	return (clnt_sock);
}

void error_handling(std::string str)
{
	std::cerr << str << std::endl; 
	exit(1);
}

int		ft_strlen(const char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return i;
}