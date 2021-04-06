#include "Command.hpp"

void	cmd_list(std::vector<std::string> split, Server *serv, Client *client)
{
	if (split.size() >= 1)
	{
		if (split.size() == 1)
		{
			std::list<Channel*> tmp = serv->getChannels();
			std::list<Channel*>::iterator chanlist = tmp.begin();
			for (std::list<Channel*>::iterator end = tmp.end(); chanlist != end; chanlist++)
				serv->fdwrite(client->getFd(), (*chanlist)->getChanName() + "\n");
			return ;
		}
	}
}