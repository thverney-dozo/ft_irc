/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_join.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaetan <gaetan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 00:50:36 by aeoithd           #+#    #+#             */
/*   Updated: 2021/06/29 13:32:37 by gaetan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void cmd_join(std::vector<std::string> split, Server *serv, Client *client)
{
	if (split.size() > 1)
	{
		if (split[1][0] == '#')
		{
			std::vector<std::string> splitlist;
			for (int i = 0; split[1][i]; i++)
			{
				if (split[1][i] == '#')
				{
					std::string str;
					for (int j = i; split[1][j] != ',' && split[1][j] != '\0' && split[1][j] != ' '; j++)
						str += split[1][j];
					splitlist.push_back(str);
					str.clear();
				}
				if (split[1][i] == ',' && split[1][i + 1] != '#')
				{
					std::string str;
					for (int j = i + 1; split[1][j] != ',' && split[1][j] != '\0' && split[1][j] != ' '; j++)
						str += split[1][j];
					serv->fdwrite(client->getFd(), ":localhost 403 " + str + " :No such channel.\r\n");
					str.clear();
				}
			}
			if (splitlist.empty() == true)
				splitlist.push_back(split[1]);
			std::vector<std::string>::iterator begin = splitlist.begin();
			for (std::vector<std::string>::iterator end = splitlist.end(); begin != end; begin++)
			{
				if (serv->checkChannels((*begin), client) == 0)
				{
					serv->createChannel((*begin), client);
					std::cout << "Client created a new channel named : " << (*begin) << std::endl;
				}
				else
				{
					Channel *chan = serv->getThisChan((*begin));
				//	std::string mods = chan->getMods();
					if (chan->getInviteOnlyStatus() == true)
					{
						if (client->getInvite(chan->getChanName()) == true)
							;
						else
						{
							serv->fdwrite(client->getFd(), "This channel is on invitation mode.\n");
							return;
						}
					}
	/*				if (mods.find('s', 0) != std::string::npos)
					{
						serv->fdwrite(client->getFd(), ":localhost 403 " + (*begin) + " :No such channel.\r\n");
						return;
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
					}*/
					//Check if there is a password case and check if the password is right
					//I dont know if this is mandatory or not
					//Maybe there is others mods to check;
					// Message d'arrivee sur le chan pour tous les clients connectes
					std::list<Channel *> list = serv->getChannels();
					std::list<Channel *>::iterator list_begin = list.begin();
					for (std::list<Channel *>::iterator list_end = list.end(); list_begin != list_end; list_begin++)
					{
						if ((*list_begin)->getChanName() == (*begin))
						{
							std::list<Client*> clist = (*list_begin)->getConnectedClients();
							std::list<Client*>::iterator it = clist.begin();
							for (std::list<Client*>::iterator ite = clist.end(); ite != it; it++)
							{
								if ((*it)->getName() == client->getName())
									return ;
							}
							client->join_channel((*list_begin));
							(*list_begin)->addClient(client);
							std::list<Client *> client_list = (*list_begin)->getConnectedClients();
							std::list<Client *>::iterator client_begin = client_list.begin();
							for (std::list<Client *>::iterator client_end = client_list.end(); client_end != client_begin; client_begin++)
							{
								if ((*client_begin)->getName() != client->getName())
									serv->fdwrite((*client_begin)->getFd(), ":" + client->getName() + "!localhost PRIVMSG " + (*begin) + " joined the channel." + "\r\n");
							}
						}
					}
					serv->fdwrite(client->getFd(), ":" + client->getName() + "!" + client->getUsername() +"@localhost JOIN :" + (*begin) + "\r\n");
					std::cout << "Client joined channel : " << (*begin) << std::endl;
				}
			}
		}
			else
				serv->fdwrite(client->getFd(), ":localhost 403 " + split[1] + " :No such channel.\r\n");
	}
	else
		serv->fdwrite(client->getFd(), ":localhost 403 " + split[1] + " :No such channel.\r\n");
}