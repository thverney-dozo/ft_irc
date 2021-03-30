/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pass.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaetan <gaetan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 15:19:56 by thverney          #+#    #+#             */
/*   Updated: 2021/03/30 11:53:47 by gaetan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmd_pass(std::vector<std::string> split, Server *serv, Client *client)
{
	if (client->getIsPassSet() == true)
		write(client->getFd(), "Already register the pass\n", 27); 	// must be changed by ERR_ALREADYREGISTRED
	if (client->getIsServer() == false) 				// if connection is a client
	{
		if (split.size() == 1)
			serv->fdwrite(client->getFd(), "/pass cmd must be followed by a password\n"); // must be changed by ERR_NEEDMOREPARAMS
		else
		{
			client->setPassword(split[1]);
			client->setPassSet(true);
		}
	}
	else // if connection is a server
	{}
}