/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaetan <gaetan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 01:44:20 by aeoithd           #+#    #+#             */
/*   Updated: 2021/05/10 17:08:31 by gaetan           ###   ########.fr       */
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

std::string Channel::getMods()
{
	return _flags;
}

void Channel::addFlag(char flag)
{
	size_t pos =_flags.find(flag);
	if (pos == std::string::npos)
		_flags += flag;
}

void Channel::removeFlag(char flag)
{
	size_t pos = _flags.find(flag);
	if (pos != std::string::npos)
		_flags.erase(pos, 1);
}

void Channel::setTopic(std::string subject)
{
	_topic = subject;
}

std::string Channel::getTopic() const
{
	return _topic;
}

void Channel::setLimit(int max)
{
	limit = max;
}

int Channel::getLimit()
{
	return limit;
}

void Channel::addUser()
{
	currentUsers += 1;
}

int Channel::getCurrentUsers()
{
	return currentUsers;
}

void Channel::userLeaveChannel()
{
	currentUsers -= 1;
}

int Channel::checkBans(Client *client)
{
	std::list<Client*>::iterator ite = banList.begin();
	for (std::list<Client*>::iterator it = banList.end(); ite != it; ite++)
	{
		if ((*ite)->getRealName() == client->getRealName())
			return 0;
	}
	return 1;
}

void Channel::addPremiumClient(Client *client)
{
	premiumUserList.push_back(client);
}

std::list<Client*> Channel::getPremiumList()
{
	return premiumUserList;
}

void Channel::removePremiumClient(Client *client)
{
	std::list<Client*> list = getPremiumList();
	std::list<Client*>::iterator it = list.begin();
	for (std::list<Client*>::iterator ite = list.end(); it != ite; it++)
	{
		if ((*it)->getName() == client->getName())
		{
			list.erase(it);
			return ;
		}
	}
}

int Channel::checkPremiumList(std::string name)
{
	std::list<Client*> list = getPremiumList();
	std::list<Client*>::iterator it = list.begin();
	for (std::list<Client*>::iterator ite = list.end(); it != ite; it++)
	{
		if ((*it)->getName() == name)
			return 1;
	}
	return 0;
}