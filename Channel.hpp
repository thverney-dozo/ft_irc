#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <list>
#include <unistd.h>
#include "Client.hpp"

class Channel
{
	private:
		std::string _name;
		std::list<Client*> _clients;

		Channel();
		Channel(const Channel &other);
		Channel &operator=(const Channel &other);

	public:
		Channel(std::string const &name, Client &client);
		virtual ~Channel();

		void addClient(Client *client);
		void clientWriteMsg(std::string msg, Client *client);

		std::string getChanName() const;
		std::list<Client*> getConnectedClients() const;
};

Channel::Channel(std::string const &name, Client &client)
{
	_name = name;
	_clients.push_back(&client);
}

Channel::~Channel()
{}

std::string Channel::getChanName() const
{
	return _name;
}

std::list<Client*> Channel::getConnectedClients() const
{
	return _clients;
}

void Channel::addClient(Client *client)
{
	_clients.push_back(client);
}

void Channel::clientWriteMsg(std::string msg, Client *client)
{
	std::list<Client*>::iterator end = this->_clients.end();
	for (std::list<Client*>::iterator begin = this->_clients.begin(); begin != end; begin++)
	{
		std::string str = _name + ": " + client->getName() + ": " + msg;
		write((*begin)->getFd(), str.c_str(), str.size()); 
	}
}

#endif