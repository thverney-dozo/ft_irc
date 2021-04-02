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
	std::string host_port;
	std::string local_port;
	std::string	host_ip;
	std::string	host_password;
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

	Server 	server(args.local_port.c_str(),
				   args.local_password,
				   args.host_ip,
				   args.host_port.c_str(),
				   args.host_password); // all setup in the construction
	std::cout << "host_ip " << server.getHostIp() << std::endl;
	std::cout << "host_port " << server.getHostPort() << std::endl;
	std::cout << "host_password " << server.getHostPassword() << std::endl;
	std::cout << "Localport " << server.getLocalPort() << std::endl;
	std::cout << "Localpassword " << server.getLocalPassword() << std::endl;

	handle_connection(&server);
	close(server.getServSock());

	return 0;
}

void    *handle_connection(Server *server)
{
	int nb_Of_Fd;

	while (1)
	{
		if((nb_Of_Fd = server->detection_select()) == -1)
			break;
		if (nb_Of_Fd == 0)
			continue;
		for (int i = 0; i < server->getFdMax() + 1; i++)
		{
			fcntl(i, F_SETFL, O_NONBLOCK);
			if (FD_ISSET(i, server->getCpyReads_addr()))
			{
				if (i == server->getServSock())
					server->connexion();
				else
				{
					int str_len;
					if ((str_len = read(i, server->getBuf(), 1024)) == 0)
						server->deconnexion(i);
					else
						server->receiveFromClient(i, str_len);
				}
			}
		}
	}
	return NULL;
}

void	print_usage()
{
		std::cout << "Usage: ./irc ip:port(:password) <localhost_port> (<local_password>)" << std::endl;
		std::cout << "or" << std::endl;
		std::cout << "Usage: " << "./irc" << " <localhost_port> "<< "(<local_password>)" << std::endl;
		exit(1);
}

t_pre_parse	args_parse(int ac, char **av)
{
	if (ac != 2 && ac != 3 && ac != 4)
		print_usage();
	t_pre_parse ret;

	std::string my_arg = av[1];
	size_t start = my_arg.find(':');
	size_t end = my_arg.rfind(':');

	if (start == std::string::npos)
	{
		if (av[3])
			print_usage();
		ret.local_port = av[1];
		if (av[2])
			ret.local_password = av[2];
	}
	else
	{
		ret.host_ip.insert(0, av[1], 0, start);
		if (start == end) // Il n'y a pas de mdp
			ret.host_port.insert(0, av[1], start + 1, my_arg.size() - start - 1);
		else
		{
			ret.host_port.insert(0, av[1], start + 1, end - start - 1);
			ret.host_password.insert(0, av[1], end + 1, my_arg.size() - end - 1);
		}		

		if (av[2])
			ret.local_port = av[2];
		else
			print_usage();
		if (av[3])
			ret.local_password = av[3];
	}
	return (ret);
}