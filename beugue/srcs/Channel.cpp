/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaetan <gaetan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 01:44:20 by aeoithd           #+#    #+#             */
/*   Updated: 2021/04/02 08:40:17 by gaetan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string const &name) : _name(name)
{}

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
	std::list<Client*>::iterator end = this->_clients.end();
	for (std::list<Client*>::iterator begin = this->_clients.begin(); begin != end; begin++)
	{
		if (client->getFd() == (*begin)->getFd())
			return ;
	}
	_clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
	std::list<Client*>::iterator begin = _clients.begin();
	for (std::list<Client*>::iterator end = _clients.end(); begin != end; begin++)
	{
		if (client->getFd() == (*begin)->getFd())
		{
			_clients.erase(begin);
			return ;
		}
	}
}

void Channel::userLeftChan(Client *client)
{
	std::list<Client*>::iterator end = this->_clients.end();
	for (std::list<Client*>::iterator begin = this->_clients.begin(); begin != end; begin++)
	{
		std::string str = "\rUser " + client->getName() + " left the channel.\n";
		if (client->getFd() == (*begin)->getFd())
			str = "\rYou left channel " + _name + "\n";
		write((*begin)->getFd(), str.c_str(), str.size()); 
	}
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

void Channel::setTopic(std::string subject)
{
	_topic = subject;
}

std::string Channel::getTopic() const
{
	return _topic;
}