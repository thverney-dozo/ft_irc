/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 15:19:53 by thverney          #+#    #+#             */
/*   Updated: 2021/06/20 21:11:13 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmd_nick(std::vector<std::string> split, Server *serv, Client *client)
{
	/* [ <compteur de distance> ] à implementer (server à server )*/

	/* Nickname is already in use ERR_NICKNAMEINUSE à implementer */
	/*  (verifier tout ce qu'il fau implementer lol) */

	(void)serv;
	if (client->getIsServer() == false) 				// if connection is a client
	{
		if (split.size() == 1)
			serv->fdwrite(client->getFd(), ":localhost 431 " + client->getName() + " NICK :No nickname given\r\n");  // ERR_NONICKNAMEGIVEN
		else
		{
			if (client->getIsNickSet() == false)
				serv->fdwrite(client->getFd(), ":" + client->getName() + "!localhost NICK " + split[1] + "\r\n");
			else
				serv->fdwrite(client->getFd(), ":" + client->getName() + " NICK " + split[1] + "\r\n");
			
			std::list<Channel*> chanlist = client->getChanList();
			std::list<Channel*>::iterator chanlist_end = chanlist.end();
			for (std::list<Channel*>::iterator chanlist_begin = chanlist.begin(); chanlist_begin != chanlist_end; chanlist_begin++)
			{
				std::list<Client*> clientlist = (*chanlist_begin)->getConnectedClients();
				std::list<Client*>::iterator clientlist_end = clientlist.end();
				for (std::list<Client*>::iterator clientlist_begin = clientlist.begin(); clientlist_end != clientlist_begin; clientlist_begin++)
					serv->fdwrite((*clientlist_begin)->getFd(), ":" + client->getName() + "!localhost PRIVMSG " + (*chanlist_begin)->getChanName() + " :" + client->getName() + " changed nickname to " + split[1] + "\r\n");
			}
			client->setNickname(split[1]);
			//CHECK NICKNAME, IF IN USE ERR_NICKNAMEINUSE
			client->setNickSet(true);
			if (client->getIsUserSet() == true)
				client->setRegister(true);
			//SEND SERVER RESPONSE TO CLIENT
		}
	}
	else // if connection is a server
	{}
}