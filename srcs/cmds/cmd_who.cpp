#include "Command.hpp"

void	cmd_who(std::vector<std::string> split, Server *serv, Client *client)
{
	(void)split;

	std::map<int, Client*> list = serv->getClients();
	std::map<int, Client*>::iterator ite = list.begin();
	for (std::map<int, Client*>::iterator it = list.end(); ite != it; ite++)
		serv->fdwrite(client->getFd(), (*ite).second->getName() + "\n");
}