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
				return ;
			}
		}
	}
}