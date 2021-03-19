#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <list>
#include <unistd.h>

class Client;

class Channel
{
	private:
		std::string _name;
		std::list<Client*> _clients;

		Channel();

	public:
		Channel(std::string const &name);
		Channel(std::string const &name, Client &client);
		Channel(const Channel &other);
		Channel &operator=(const Channel &other);
		virtual ~Channel();

		void addClient(Client *client);
		void clientWriteMsg(std::string msg, Client *client);

		std::string getChanName() const;
		std::list<Client*> getConnectedClients() const;
};


Channel::Channel(std::string const &name)
{
	this->_name = name;
}

Channel::Channel(const Channel &other)
{
	*this = other;
}

Channel &Channel::operator=(const Channel &other)
{
	_name = other._name;
	return *this;
}

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

#include "Client.hpp"

#endif