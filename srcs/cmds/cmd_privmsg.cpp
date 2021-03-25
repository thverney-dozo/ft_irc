#include "Command.hpp"

void	cmd_privmsg(std::vector<std::string> split, Server *serv, Client *client)
{
	std::vector<Client*> list = serv->getClients();
	std::vector<Client*>::iterator ite = list.begin();
	for (std::vector<Client*>::iterator it = list.end(); it != ite; ite++)
	{
		if ((*ite)->getName() == client->getName())
			continue;
		if((*ite)->getName() == split[1])
		{
			std::string message;
			std::vector<std::string>::iterator ite2 = split.begin();
			for (std::vector<std::string>::iterator it = split.end(); ite2 != it; ite2++)
			{
				if ((*ite2)[0] == '/' || (*ite2) == (*ite)->getName())
					continue;
				message += (*ite2);
				message += " ";
			}
			std::string realmessage = "From " + client->getName() + ": " + message;
			serv->fdwrite((*ite)->getFd(), realmessage);
			serv->fdwrite(client->getFd(), "\033[A\33[2KT\r");
			serv->fdwrite(client->getFd(), "To " + (*ite)->getName() + ": " + message);
			return ;
		}
	}
	serv->fdwrite(client->getFd(), "Error: user \"" + split[1] + "\" does not exist\n");
}