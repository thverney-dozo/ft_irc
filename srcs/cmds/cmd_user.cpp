/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_user.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 15:19:59 by thverney          #+#    #+#             */
/*   Updated: 2021/06/29 15:38:17 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"
#include <ctime>

void	cmd_user(std::vector<std::string> split, Server *serv, Client *client)
{
	(void)serv;
	if (client->getIsServer() == false) 				// if connection is a client
	{
		if (client->getIsUserSet() == true)
			serv->fdwrite(client->getFd(), ":localhost 462 " + client->getName() + " USER :Unauthorized command (already registered)\r\n"); //ERR_NEEDMOREPARAMS
		else if (split.size() == 1)
			serv->fdwrite(client->getFd(), ":localhost 461 " + client->getName() + " USER :Not enough parameters\r\n"); //ERR_NEEDMOREPARAMS
		else if (split.size() >= 4 && client->getIsUserSet() == false)
		{
			client->setUsername(split[1]);
			client->setHostName(split[2]);
			client->setServerName(split[3]);
			client->setRealName(split[4]);
			client->setUserSet(true);
			client->setHostName(true);
			client->setServerName(true);
			client->setRealName(true);
			if (client->getIsNickSet() == true)
			{
				time_t     now = time(0);
    			struct tm  tstruct;
    			char       buf[80];
    			tstruct = *localtime(&now);
    			strftime(buf, sizeof(buf), "%c", &tstruct);

				serv->fdwrite(client->getFd(), ":localhost 001 " + client->getName() + " :Welcome to the server\r\n");
				serv->fdwrite(client->getFd(), ":localhost 002 " + client->getName() + " :Your host is localhost, running version 1.0\r\n");
				serv->fdwrite(client->getFd(), ":localhost 003 " + client->getName() + " :This server was created " + buf +"\r\n");
				serv->fdwrite(client->getFd(), ":localhost 004 " + client->getName() + " :localhost 1.0 . .\r\n");
				serv->fdwrite(client->getFd(), ":localhost 005 " + client->getName() + " :" + serv->getLocalPort() +"\r\n");
			}
			if (client->getIsNickSet() == true)
				client->setRegister(true);
			//exemple   USER guest 8 * :Ronnie Reagan
		}
		// else
		// {
		// 	client->setUsername(split[1]);
		// 	client->setUserSet(true);
		// }

	}
}