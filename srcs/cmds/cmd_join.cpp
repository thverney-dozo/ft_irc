/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 00:50:36 by aeoithd           #+#    #+#             */
/*   Updated: 2021/06/18 14:42:06 by thverney         ###   ########.fr       */
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
			{
				Channel *chan = serv->getThisChan(split[1]);
				std::string mods = chan->getMods();
				if (mods.find('i', 0) != std::string::npos)
				{
					serv->fdwrite(client->getFd(), "This channel is on invitation mode.\n");
					return ;
				}
				if (mods.find('s', 0) != std::string::npos)
				{
					serv->fdwrite(client->getFd(), "No such channel.\n");
					return ;
				}
				if (mods.find('l', 0) != std::string::npos)
				{
					if (chan->getLimit() == chan->getCurrentUsers())
						serv->fdwrite(client->getFd(), "Channel is full.\n");
					else
					{
						chan->addClient(client);
						chan->addUser();
					}
				}
				if (mods.find('b', 0) != std::string::npos)
				{
					if (!chan->checkBans(client))
						serv->fdwrite(client->getFd(), "You are banned from the channel.\n");
					else
					{
						chan->addClient(client);
						chan->addUser();
					}
				}
				//Check if there is a password case and check if the password is right
				//I dont know if this is mandatory or not
				//Maybe there is others mods to check;
				serv->fdwrite(client->getFd(), ":" + client->getName() + "!localhost JOIN " + split[1] + "\r\n");
				// Message d'arrivee sur le chan pour tous les clients connectes
				std::list<Channel*> list = serv->getChannels();
				std::list<Channel*>::iterator list_begin = list.begin();
				for (std::list<Channel*>::iterator list_end = list.end(); list_begin != list_end; list_begin++)
				{
					if ((*list_begin)->getChanName() == split[1])
					{
						std::list<Client*> client_list = (*list_begin)->getConnectedClients();
						std::list<Client*>::iterator client_begin = client_list.begin();
						for (std::list<Client*>::iterator client_end = client_list.end(); client_end != client_begin; client_begin++)
							serv->fdwrite((*client_begin)->getFd(), ":" + client->getName() + "!localhost PRIVMSG " + split[1] + " joined the channel." + "\r\n");
					}
				}
				std::cout << "Client joined channel : " << split[1] << std::endl;
			}
		}
		else
			serv->fdwrite(client->getFd(), "Wrong use of JOIN command.\n");
	}
	else
		serv->fdwrite(client->getFd(), "Wrong use of JOIN command.\n");
}