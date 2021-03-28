/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_user.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 15:19:59 by thverney          #+#    #+#             */
/*   Updated: 2021/03/28 15:33:51 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmd_user(std::vector<std::string> split, Server *serv, Client *client)
{
	(void)serv;
	if (client->getIsServer() == false) 				// if connection is a client
	{
		if (split[1].empty())
			write(client->getFd(), "/user cmd must be followed by a username\n", 42); // must be changed by ERR_NONICKNAMEGIVEN
		else
		{
			client->setUsername(split[1]);
			client->setUserSet(true);
		}
	}
	else // if connection is a server
	{}
}