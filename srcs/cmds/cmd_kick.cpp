#include "Command.hpp"

void	cmd_kick(std::vector<std::string> split, Server *serv, Client *client)
{
	if (split.size() >= 3)
	{
		if (split.size() == 3)
		{
			Channel * chan = serv->getThisChan(split[1]);
			if (chan->checkPremiumList(client->getName()) == 0)
			{
				serv->fdwrite(client->getFd(), "Error, you're not an operator.\n");
				return ;
			}
			if (chan == nullptr)
			{
				//ERR_NOSUCHCHAN
				serv->fdwrite(client->getFd(), "Error, no such channel.\n");
				return;
			}
			std::list<Client*> list = chan->getConnectedClients();
			std::list<Client*>::iterator it = list.begin();
			for (std::list<Client*>::iterator ite = list.end(); it != ite; it++)
			{
				if ((*it)->getName() == split[2])
				{
					serv->fdwrite((*it)->getFd(), ":" + (*it)->getName() + "!localhost PART " + chan->getChanName() + "\r\n");
					std::list<Client*>::iterator begin = list.begin();
					for (std::list<Client*>::iterator end = list.end(); begin != end; begin ++)
						serv->fdwrite((*begin)->getFd(), ":" + split[2] + "!localhost PRIVMSG " + chan->getChanName() + " : got kicked by " + client->getName() + "." +  "\r\n");
					return ;
				}
			}
		}
		if (split.size() == 4)
		{
			Channel * chan = serv->getThisChan(split[1]);
			if (chan->checkPremiumList(client->getName()) == 0)
			{
				serv->fdwrite(client->getFd(), "Error, you're not an operator.\n");
				return ;
			}
			if (chan == 0)
			{
				//ERR_NOSUCHCHAN
				serv->fdwrite(client->getFd(), "Error, no such channel.\n");
				return;
			}
			std::list<Client*> list = chan->getConnectedClients();
			std::list<Client*>::iterator it = list.begin();
			for (std::list<Client*>::iterator ite = list.end(); it != ite; it++)
			{
				if ((*it)->getName() == split[2])
				{
					serv->fdwrite((*it)->getFd(), ":" + (*it)->getName() + "!localhost PART " + chan->getChanName() + "\r\n");
					std::list<Client*>::iterator begin = list.begin();
					for (std::list<Client*>::iterator end = list.end(); begin != end; begin ++)
						serv->fdwrite((*begin)->getFd(), ":" + split[2] + "!localhost PRIVMSG " + chan->getChanName() + " : got kicked by " + client->getName() + " : " + split[3] +  "\r\n");
					return ;
				}
			}
		}
	}
	else
		serv->fdwrite(client->getFd(), "Bite.\n");
		//ERR_NEEDMOREPARAMS
}