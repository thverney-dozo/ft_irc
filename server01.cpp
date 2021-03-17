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
#include "Server.hpp"

void	init_bin_args(int argc, char *bin);
void    *handle_connection(Server *server);

int 	main(int argc, char **argv)
{
	init_bin_args(argc, argv[0]);

	Server server(argv[1], argv[2]); // all setup in the construction

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

void	init_bin_args(int ac, char *av)
{
	if (ac != 3)
	{
		std::cout << "Usage: " << av << " <port>"<< "<password>" << std::endl;
		exit(1);
	}
}