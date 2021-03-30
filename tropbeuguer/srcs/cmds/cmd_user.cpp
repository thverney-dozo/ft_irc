/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_user.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaetan <gaetan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 15:19:59 by thverney          #+#    #+#             */
/*   Updated: 2021/03/30 11:56:16 by gaetan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmd_user(std::vector<std::string> split, Server *serv, Client *client)
{
	(void)serv;
	if (client->getIsServer() == false) 				// if connection is a client
	{
		if (split.size() == 1)
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