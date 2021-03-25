#include "Command.hpp"

void	cmd_who(std::vector<std::string> split, Server *serv, Client *client)
{
	(void)split;
	std::vector<Client*> list = serv->getClients();
	std::vector<Client*>::iterator ite = list.begin();
	for (std::vector<Client*>::iterator it = list.end(); ite != it; ite++)
		serv->fdwrite(client->getFd(), (*ite)->getName() + "\n");
}