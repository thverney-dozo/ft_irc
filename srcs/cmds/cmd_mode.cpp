#include "Command.hpp"
#include <cstdlib>

void setMods(std::string name, char sign, char mod, Server *serv, Client *client)
{
	std::list<Channel*> channels = serv->getChannels();
	std::list<Channel*>::iterator begin = channels.begin();
	for (std::list<Channel*>::iterator end = channels.end(); begin != end; begin++)
	{
		if(name == (*begin)->getChanName())
			{
				if (sign == '+' && mod == 'i')
				{
					if ((*begin)->checkPremiumList(client->getName()) == 1)
						(*begin)->setInviteOnly(true);
					else
					{
						serv->fdwrite(client->getFd(), "Error, you are not a channel operator.\n");
						return ;
					}
					
				}
				else if (sign == '-' && mod == 'i')
				{
					if ((*begin)->checkPremiumList(client->getName()) == 1)
						(*begin)->setInviteOnly(false);
					else
					{
						serv->fdwrite(client->getFd(), "Error, you are not a channel operator.\n");
						return ;
					}
				}
			}
	}
}

void setModsToChan(std::string name, char sign, char mod, std::string param, Client *client, Server *serv)
{
	Channel *chan = serv->getThisChan(name);
	
	//name = chan name
	//sign = sign
	//mod = mod
	//param = username
	std::list<Client*> list = chan->getConnectedClients();
	std::list<Client*>::iterator it = list.begin();
	if (mod == 'o')
	{
		if ((*it)->getName() == param)
		{
			if (mod == '+')
			{
				if (chan->checkPremiumList((*it)->getName()) == 0)
					chan->addPremiumClient((*it));
			}
			else if (mod == '-')
			{
				if (chan->checkPremiumList((*it)) == 0)
					chan->removePremiumClient((*it));
			}
		}
	}
	else if (mod == 'l')
	{
		if (atoi(param.c_str()) < chan->getCurrentUsers())
			serv->fdwrite(client->getFd(), "Error, cannot set limit to" + param + ".\n");
		else
			chan->setLimit(atoi(param.c_str()));
	}
	else if (mod == 'v')
	{
		if (sign == '+')
		{
			if (chan->checkPremiumList(param))
			{
				std::map<int, Client *> map = serv->getClients();
				//jsais pas use une map, faut juste trouver le client en question;
				// une fois que c'est fait tu fais un chan->addPremiumClient(client)
			}
		}
		if (sign == '-')
		{
			if (chan->checkPremiumList(param))
			{
				std::map<int, Client *> map = serv->getClients();
				//pareil ici mais a l'inverse du coup
			}
		}
		//Finir B et K si y a besoin de le faire
	}
}

//void setBans(std::string name, char sign, char mod, std::string user, std::string mask, Client *client, Server *serv)
//{
//	
//}

void cmd_mode(std::vector<std::string> split, Server *serv, Client *client)
{
	if (split.size() > 2)
	{
		std::cout << split[1] << "|" << split[2] << "|" << std::endl;
		if (serv->checkChannelList(split[1]) == 1)
		{
			if (split[2][0] != '+' && split[2][0] != '-')
				serv->fdwrite(client->getFd(), "Error, invalid params.\n");
			if ((split[2][1] == 'p' || split[2][1] == 's' || split[2][1] == 't'
				|| split[2][1] == 'n' || split[2][1] == 'm' || split[2][1] == 'v' || split[2][1] == 'i') && split.size() == 3)
					setMods(split[1], split[2][0], split[2][1], serv, client);
			else if ((split[2][1] == 'o' || split[2][1] == 'l' || split[2][1] == 'v' || split[2][1] == 'b') && split.size() == 4)
				setModsToChan(split[1], split[2][0], split[2][1], split[3], client, serv);
		//	else if (split[2][1] == 'b' && split.size() == 5)
			//	setBans(split[1], split[2][0], split[2][1], split[3], split[4], client, serv);
		}
		else if (split[1] == client->getName())
		{
			if (split[2][0] != '+' && split[2][0] != '-')
				serv->fdwrite(client->getFd(), "Error, invalid params.\n");
			if (split[2][1] == 'o')
			{
				if (split[2][0] == '+')
					client->addMod(c);
				else
					client->remMod(c);
			}
		}
		else
			serv->fdwrite(client->getFd(), "Error, wrong usage of MODE command.\n");
	}
}