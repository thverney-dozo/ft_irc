#include "Command.hpp"

void	cmd_privmsg(std::vector<std::string> split, Server *serv, Client *client)
{
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
			for (std::vector<std::string>::iterator it = split.end(); ite2 != it; ite2++)
			{
				if ((*ite2)[0] == '/' || (*ite2) == (*ite).second->getName())
					continue;
				message += (*ite2);
				message += " ";
			}
			std::string realmessage = "From " + client->getName() + ": " + message + '\n';
			serv->fdwrite((*ite).second->getFd(), realmessage);
			serv->fdwrite(client->getFd(), "\033[A\33[2KT\r");
			serv->fdwrite(client->getFd(), "To " + (*ite).second->getName() + ": " + message + '\n');
			return ;
		}
	}
	serv->fdwrite(client->getFd(), "Error: user \"" + split[1] + "\" does not exist\n");
}