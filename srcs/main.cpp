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
#include <map>
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"

typedef struct s_pre_parse
{
	std::string local_port;
	std::string local_password;
}				t_pre_parse;

void    	*handle_connection(Server *server);
t_pre_parse	args_parse(int ac, char **av);
int 	main(int ac, char **av)
{
	/*
	* Creation server simple
	** av[0] == "./irc"
	** av[1] == "port" (de mon server)
	** av[2] == "mdp" (mdp optionnel de mon server)
	ou
	* Connections server@server
	** av[0] == "./irc"
	** av[1] == "ip:port:mdp" (mdp optionnel)
	** av[2] == "port" (de mon server)
	** av[3] == "mdp" (mdp optionnel de mon server)
	*/
	t_pre_parse args = args_parse(ac, av);

	std::cout << "segfault1.1" << std::endl;
	Server 	server(args.local_port.c_str(),
				   args.local_password); // all setup in the construction
	std::cout << "Localport :" << server.getLocalPort() << std::endl;
	std::cout << "Localpassword :" << server.getLocalPassword() << std::endl;

	handle_connection(&server);
	close(server.getServSock());

	return 0;
}

void	insert_read_data_in_client_buffer(Server *server, char *buffer_read, int fd_i)
{
	if (server->clients_buffer.find(fd_i) != server->clients_buffer.end())
	{
		server->clients_buffer.find(fd_i)->second.insert(server->clients_buffer.find(fd_i)->second.size(), buffer_read);
		// std::cout << "SIZE OF BUF : " << server->clients_buffer.find(fd_i)->second.size() << std::endl;
	}
	else
		server->clients_buffer.insert(std::pair<int, std::string>(fd_i, buffer_read));
}

void    *handle_connection(Server *server)
{
	int nb_Of_Fd;

	while (1)
	{
		// std::cout << "_______________begin of handle connection________________" << std::endl;
		if((nb_Of_Fd = server->detection_select()) == -1)
			break;
		// std::cout << std::endl;
		if (nb_Of_Fd == 0)
			continue;
		for (int i = 0; i < server->getFdMax() + 1; i++)
		{
			if (FD_ISSET(i, server->getCpyReads_addr()))
			{
				// std::cout << "{---SEGFAULT 14?----}" << std::endl;
				if (i == server->getServSock())
					server->connexion();
				else
				{
					int str_len;
					char buffer_read[2048];
					memset(buffer_read, 0, 2048);
					str_len = read(i, buffer_read, 2048);
					if (str_len <= 0)
					{
						server->deconnexion(i);
						break;
					}
					insert_read_data_in_client_buffer(server, buffer_read, i);
					if (str_len < 2048 && buffer_read[str_len - 1] == '\n')
					{
						// std::cout << "str_len < 2048" << std::endl;
						server->Sender = server->getClients().find(i)->second; // current client to deal with
						server->receiveFromClient(i);
						// std::cout << server->clients_buffer.find(i)->second;
						// std::cout << "{---SEGFAULT 12?----}" << std::endl;
						server->clients_buffer.find(i)->second.clear();
					}
					// else
					// 	std::cout << "str_len > 2048" << std::endl;
					// std::cout << "{---SEGFAULT 13?----}" << std::endl;
					
				}
			}
		}
	}
	return NULL;
}

void	print_usage()
{
		std::cout << "Usage: " << "./irc" << " <localhost_port> "<< "(<local_password>)" << std::endl;
		exit(1);
}

t_pre_parse	args_parse(int ac, char **av)
{
	if (ac != 2 && ac != 3)
		print_usage();
	t_pre_parse ret;

	ret.local_port = av[1];
	if (ac == 3)
		ret.local_password = av[2];
	return (ret);
}