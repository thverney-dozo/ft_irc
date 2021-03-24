/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeoithd <aeoithd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 00:50:36 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/24 02:16:32 by aeoithd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmd_join(std::vector<std::string> split, Server *serv, Client *client)
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