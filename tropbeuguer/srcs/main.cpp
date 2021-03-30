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

	if (ac != 3 || ac != 4){
		std::cout << "Usage: " << av[0] << " <port>"<< "<password>" << std::endl;
		exit(1);
	}

	Server 	server(args.local_port.c_str(),
				   args.local_password,
				   args.host_ip,
				   args.host_port.c_str(),
				   args.host_password); // all setup in the construction
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

t_pre_parse	args_parse(int ac, char **av)
{
	if (ac != 3 || ac != 4){
		std::cout << "Usage: " << av[0] << " <port>"<< "<password>" << std::endl;
		exit(1);
	}
	t_pre_parse ret;

	if (ac == 3)
	{
		ret.local_port = av[1];		
		ret.local_password = av[2];
		ret.host_ip = "NULL";
		ret.host_port = "NULL";
		ret.host_password = "NULL";
	}
	else
	{
		int is_pass_enable_host = 0;
		for (int i = 0; av[1][i]; i++)
			if (av[1][i] == ':')
				is_pass_enable_host++;
		if (is_pass_enable_host != 1 && is_pass_enable_host != 2)
		{
			std::cout << "Usage: ./irc ip:port(:password) localhost local_password" << std::endl;
			exit(1);
		}
		int tmp = 0;
		int old_tmp = 0;
		while (av[1][tmp] && av[1][tmp++] != ':')
			;
		ret.host_ip = std::string(av[1], 0, tmp);
		old_tmp = tmp + 1;
		while (av[1][tmp] && av[1][tmp++] != ':')
			;
		ret.host_port = std::string(av[1], old_tmp, tmp);
		if (is_pass_enable_host == 2)
		{
			old_tmp = tmp + 1;
			while (av[1][tmp] && av[1][tmp++] != ':')
				;
			ret.host_password = std::string(av[1], old_tmp, tmp);
		}
		else
			ret.host_password = "NULL";
		ret.local_port = av[2];
		ret.local_password = av[3];		
	}
}