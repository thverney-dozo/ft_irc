#include "Command.hpp"

void	cmd_invite(std::vector<std::string> split, Server *serv, Client *client)
{
	if (split.size() >= 3)
	{
		Channel * chan = serv->getThisChan(split[2]);
		//Check si le chan existe
		if (chan)
		{
			//Check si le client qui lance la commande est OP
			if (chan->checkPremiumList(client->getName()) == 0)
			{
				//ERR_CHANOPRIVISNEEDED
				return ;
			}
			std::list<Client*> cllist = chan->getConnectedClients();
			std::list<Client*>::iterator begin = cllist.begin();
			for (std::list<Client*>::iterator end = cllist.end(); begin != end; begin++)
			{
				//Check si le client qui lance la commande est sur le chan;
				if ((*begin)->getName() == client->getName())
				{
					std::map<int, Client*> clientlist = serv->getClients();
					std::map<int, Client*>::iterator it = clientlist.begin();
					for (std::map<int, Client*>::iterator ite = clientlist.end(); it != ite; it++)
					{
						//Check si l'utilisateur invite est bien sur le serv
						if ((*it).second->getName() == split[1])
						{
							(*it).second->addInvite(split[2]);
							serv->fdwrite((*it).second->getFd(), "You recieved an invitation to join the channel : " + chan->getChanName() + "\r\n");
							// SUCCESS
							return ;
						}
					}
					//ERR NO SUCH NICK
					return ;
				}
			}
			//ERR NOTONCHANNEL
			return ;
		}
		else
		{
			//ERR NO SUCH CHAN
		}
	}
	//else
		//ERR NEEDMOREPARAMS
}