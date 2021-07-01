#include "Command.hpp"

void	cmd_privmsg(std::vector<std::string> split, Server *serv, Client *client)
{
	if (split.size() == 1)
	{
		serv->fdwrite(client->getFd(), ":localhost 411 " + client->getName() + " PRIVMSG :No recipient given (<command>)\r\n");  // ERR_NONICKNAMEGIVEN
		return;
	}
	if (split.size() == 2)
	{
		serv->fdwrite(client->getFd(), ":localhost 412 " + client->getName() + " PRIVMSG :No text to send\r\n");  // ERR_NONICKNAMEGIVEN
		return;
	}
	std::map<int, Client*> list = serv->getClients();
	std::map<int, Client*>::iterator ite = list.begin();
	for (std::map<int, Client*>::iterator it = list.end(); it != ite; ite++)
	{
		if ((*ite).second->getName() == client->getName())
			continue;
		if((*ite).second->getName() == split[1])
		{
			std::string message;
			std::vector<std::string>::iterator ite2 = split.begin();
			std::vector<std::string>::iterator it = split.end();
			if (split.begin() != split.end())
				ite2++;
			for (; ite2 != it; ite2++)
			{
				if ((*ite2)[0] == '/' || (*ite2) == (*ite).second->getName())
					continue;
				message += (*ite2);
				message += " ";
			}
			serv->fdwrite((*ite).second->getFd(), ":" + client->getName() + "!localhost PRIVMSG " + (*ite).second->getName() + " :" + message + "\r\n");
			return ;
		}
	}
	std::list<Channel*> chanlist = serv->getChannels();
	std::list<Channel*>::iterator itex = chanlist.end();
	for (std::list<Channel*>::iterator it = chanlist.begin(); it != itex; it++)
	{
		if ((*it)->getChanName() == split[1])
		{
			{
				std::list<Client*> clientlist = (*it)->getConnectedClients();
				std::list<Client*>::iterator begin = clientlist.begin();
				std::list<Client*>::iterator end = clientlist.end();
				for (; begin != end; begin++)
				{
					if (client->getFd() == (*begin)->getFd())
						break;
				}
				if (begin == end)
				{
					serv->fdwrite(client->getFd(), ":localhost 404 " + client->getName() + " PRIVMSG " + split[1] +  " :Cannot send to channel\r\n");
					return ;
				}
			}

			std::string message;
			std::vector<std::string>::iterator ite2 = split.begin();
			std::vector<std::string>::iterator iter = split.end();
			if (split.begin() != split.end())
				ite2++;
			for (; ite2 != iter; ite2++)
			{
				if((*ite2)[0] == '/' || (*ite2) == (*it)->getChanName())
					continue;
				message += (*ite2);
				message += " ";
			}
			std::list<Client*> clientlist = (*it)->getConnectedClients();
			std::list<Client*>::iterator begin = clientlist.begin();
			for (std::list<Client*>::iterator end = clientlist.end(); begin != end; begin++)
			{
				if (client->getFd() == (*begin)->getFd())
					continue;
				serv->fdwrite((*begin)->getFd(), ":" + client->getName() + "!localhost PRIVMSG " + (*it)->getChanName() + " :" + message + "\r\n");
			}
			return ;
		}
	}
	//CHECK IF ITS A USER OR A CHAN, IF ITS A CHAN ->ERR_CANNOTSENDTOCHAN
	if (split[1][0] == '#')
		serv->fdwrite(client->getFd(), ":localhost 404 " + client->getName() + " PRIVMSG " + split[1] +  " :Cannot send to channel\r\n"); // ERR_CANNOTSENDTOCHAN
	else
		serv->fdwrite(client->getFd(), ":localhost 401 " + client->getName() + " PRIVMSG " + split[1] +  " :No such nick/channel\r\n"); // ERR_NOSUCHNICK
}