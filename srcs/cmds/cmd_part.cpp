#include "Command.hpp"

void	cmd_part(std::vector<std::string> split, Server *serv, Client *client)
{
	if (split.size() > 1)
	{
		if (split[1][0] == '#')
		{
			if (serv->checkChannelList(split[1]) == 1)
			{
				client->leave_channel(split[1]);
				std::list<Channel*> list = serv->getChannels();
				std::list<Channel*>::iterator begin = list.begin();
				for (std::list<Channel*>::iterator end = list.end(); begin != end; begin++)
				{
					if ((*begin)->getChanName() == split[1])
					{
						std::string message;
						std::vector<std::string>::iterator ite2 = split.begin();
						std::vector<std::string>::iterator it = split.end();
						if (split.begin() != split.end())
							ite2++;
						for (; ite2 != it; ite2++)
						{
							if ((*ite2)[0] == '/' || (*ite2) == (*begin)->getChanName())
								continue;
							message += (*ite2);
							message += " ";
						}
						std::cout << ":" + client->getName() + "!localhost PART " + (*begin)->getChanName() + " " << std::endl;
						if (!message.empty())
						{
							std::list<Client*> clients_co = (*begin)->getConnectedClients();
							std::list<Client*>::iterator ite3 = clients_co.end();
							for (std::list<Client*>::iterator it3 = clients_co.begin(); it3 != ite3; it3++)
							{
								if ((*it3)->getFd() != client->getFd())
									serv->fdwrite((*it3)->getFd(), ":" + client->getName() + "!localhost PRIVMSG " + (*begin)->getChanName() + " :" + client->getName() +" is exiting Channel: " + message + "\r\n");
							}
							serv->fdwrite(client->getFd(), ":" + client->getName() + "!localhost PART " + (*begin)->getChanName() + " " + message + "\r\n");
						}
						else
							serv->fdwrite(client->getFd(), ":" + client->getName() + "!localhost PART " + (*begin)->getChanName() + "\r\n");
						(*begin)->removeClient(client);
					}
				}
				return ;
			// ERR_NOTONCHANNEL
			}
			else
			// ERR_NOSUCHCHANNEL
				serv->fdwrite(client->getFd(), "Error: '" + split[1] +  "' no such channel\n");
		}
	}
	// else
	// ERR_NEEDMOREPARAMS
}