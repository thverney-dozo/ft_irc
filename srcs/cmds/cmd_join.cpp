/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaetan <gaetan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 00:50:36 by aeoithd           #+#    #+#             */
/*   Updated: 2021/04/02 08:29:23 by gaetan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmd_join(std::vector<std::string> split, Server *serv, Client *client)
{
	if (split.size() > 1)
	{
  	  if (split[1][0] == '#')
		{
			if (serv->checkChannels(split[1], client) == 0)
			{
				serv->createChannel(split[1], client);
				std::cout << "Client created a new channel named : " << split[1] << std::endl;
			}
			else
				std::cout << "Client joined channel : " << split[1] << std::endl;
		}
		else
			serv->fdwrite(client->getFd(), "Wrong use of JOIN command.\n");
	}
	else
		serv->fdwrite(client->getFd(), "Wrong use of JOIN command.\n");
}