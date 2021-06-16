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
				client->setCurrentChan("nullptr");
				std::list<Channel*> list = serv->getChannels();
				std::list<Channel*>::iterator begin = list.begin();
				for (std::list<Channel*>::iterator end = list.end(); begin != end; begin++)
				{
					if ((*begin)->getChanName() == split[1])
						(*begin)->removeClient(client);
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