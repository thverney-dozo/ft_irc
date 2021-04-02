#include "Command.hpp"

void	cmd_topic(std::vector<std::string> split, Server *serv, Client *client)
{
	if (split.size() > 1)
	{
		std::vector<std::string>::iterator check;
		std::list<Channel*> tmp = serv->getChannels();
		std::vector<std::string>::iterator begin = split.begin();
		int i = 0;
		for (std::vector<std::string>::iterator end = split.end(); begin != end; begin++)
		{
			if ((*begin)[0] == '#')
			{
				std::list<Channel*>::iterator chanlist = tmp.begin();
				for (std::list<Channel*>::iterator chanend = tmp.end(); chanlist != chanend; chanlist++)
				{
					if((*chanlist)->getChanName() == (*begin))
					{
						begin++;
						if (begin != end)
							i = 2;
						else 
						{
							begin--;
							std::list<Channel*>::iterator chanlist = tmp.begin();
							for (std::list<Channel*>::iterator end = tmp.end(); chanlist != end; chanlist++)
							{
								if((*chanlist)->getChanName() == (*begin))
								{
									if ((*chanlist)->getTopic().empty() == true)
										serv->fdwrite(client->getFd(), "Error :No topic\n");
									else
										serv->fdwrite(client->getFd(), (*chanlist)->getTopic() + "\n");
								}
							}
						return ;
						}
						begin--;
						check = begin;
					}
				}
				continue ;
			}
			else if (i == 2)
				{
					std::string subject;
					for (std::vector<std::string>::iterator end = split.end(); begin != end; begin++)
						subject += (*begin);
					std::cout << subject << std::endl;
					std::list<Channel*>::iterator chanlist = tmp.begin();
					for (std::list<Channel*>::iterator end = tmp.end(); chanlist != end; chanlist++)
					{
						if((*chanlist)->getChanName() == (*check))
							(*chanlist)->setTopic(subject);
					}
					return ;
				}
		}
	}
	else
		serv->fdwrite(client->getFd(), "Error :Need more params\n");
}