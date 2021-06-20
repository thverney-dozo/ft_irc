/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_user.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 15:19:59 by thverney          #+#    #+#             */
/*   Updated: 2021/06/20 21:11:33 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

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
				serv->fdwrite(client->getFd(), ":localhost 001 " + client->getName() + " :Welcome to the server\r\n");
			if (client->getIsNickSet() == true)
				client->setRegister(true);
			// NEED TO CHECK IF USER ALREADY EXISTS, IF HE IS, ERR_ALREADYREGISTER
			//USER guest 8 * :Ronnie Reagan par exemple
		}
		// else
		// {
		// 	client->setUsername(split[1]);
		// 	client->setUserSet(true);
		// }

	}
}