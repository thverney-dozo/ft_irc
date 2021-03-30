/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_nick.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaetan <gaetan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 15:19:53 by thverney          #+#    #+#             */
/*   Updated: 2021/03/30 11:55:58 by gaetan           ###   ########.fr       */
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
			write(client->getFd(), "/nick cmd must be followed by a nickname\n", 42); // must be changed by ERR_NONICKNAMEGIVEN
		else
		{
			client->setNickname(split[1]);
			client->setNickSet(true);
		}
	}
	else // if connection is a server
	{}
}