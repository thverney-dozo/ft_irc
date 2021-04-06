#include "Command.hpp"

void	cmd_quit(std::vector<std::string> split, Server *serv, Client *client)
{
	std::string leave;
	if (split.size() == 1)
	{
		leave = "QUIT :disconnected\n";
		client->write_on_all_chans(leave);
	}
	else
	{
		leave = "QUIT :";
		std::vector<std::string>::iterator begin = split.begin();
		for (std::vector<std::string>::iterator end = split.end(); begin != end; begin++)
		{
			if ((*begin)[0] == '/')
				continue;
			leave += (*begin) + ' ';	
		}
		leave += "\n";
		client->write_on_all_chans(leave);
	}
	client->setConnectionStatus(false);
	serv->deconnexion(client->getFd());
}