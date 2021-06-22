/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_pass.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 15:19:56 by thverney          #+#    #+#             */
/*   Updated: 2021/06/22 15:38:56 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmd_pass(std::vector<std::string> split, Server *serv, Client *client)
{
	if (client->getIsPassSet() == true)
		serv->fdwrite(client->getFd(), ":localhost 462 " + client->getName() + " PASS :You may not reregister\r\n"); // ERR_ALREADYREGISTRED
	if (client->getIsServer() == false)
	{
		if (split.size() == 1)
			serv->fdwrite(client->getFd(), ":localhost 461 " + client->getName() + " PASS :<command> :Not enough parameters\r\n"); // ERR_NEEDMOREPARAMS
		else
		{
			if (split[1] == serv->getLocalPassword())
				client->setPassSet(true);
		}
	}
}