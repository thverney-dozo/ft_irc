/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeoithd <aeoithd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 02:19:44 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/24 02:19:51 by aeoithd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : fd(), addr(), adr_sz(), nickname("noName"), stat(false) , pass(false), current_channel("nullptr")
{}

Client::Client(int fd, struct sockaddr_in addr, socklen_t adr_sz) : fd(fd), addr(addr), adr_sz(adr_sz), stat(false), pass(false), current_channel("nullptr")
{}

Client::~Client()
{}

void				Client::setCurrentChan(std::string name)
{
	current_channel = name;
}

void                Client::join_channel(Channel *channel)
{
    this->channels.push_back(channel);
}

void    Client::setStat(bool is_set)
{
    this->stat = is_set;
}

void    Client::setName(std::string &name)
{
    std::string new_name;
    
    for (std::string::size_type i = 0; i < name.size(); i++) {
        if (name[i] != '\n')
            new_name.push_back(name[i]);
    }
    this->nickname = new_name;
}

int     Client::getFd() const
{
    return (this->fd);
}

std::string Client::getCurrentChan() const
{
	return current_channel;
}

std::string const &Client::getName() const
{
    return (this->nickname);
}

bool    Client::getStat() const
{
    return (this->stat);
}

void Client::setPass(bool is_set)
{
	pass = is_set;
}

bool Client::getPass() const
{
	return pass;
}

/*void    Client::clear_info()
{
        int                 fd;
        struct sockaddr_in  addr;
        socklen_t           adr_sz;
        std::string         nickname;
        bool                stat; // client has named himself;
}*/

int Client::getPass(std::string password, int clnt_sock, char tmp[1024])
{
	(void)clnt_sock;	
	std::string marre = tmp;
	if (password == marre)
		return 1;
	return 0;
}