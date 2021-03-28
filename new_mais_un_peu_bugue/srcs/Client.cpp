/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 02:19:44 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/28 15:40:39 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : fd(), addr(), adr_sz(), current_channel("nullptr")
{
    this->is_pass_set = false;
    this->is_user_set = false;
    this->is_nick_set = false;
    this->is_register = false;
}

Client::Client(int fd, struct sockaddr_in addr, socklen_t adr_sz) : fd(fd), addr(addr), adr_sz(adr_sz), current_channel("nullptr")
{
    this->is_pass_set = false;
    this->is_user_set = false;
    this->is_nick_set = false;
    this->is_register = false;
}

Client::~Client()
{}

void                Client::join_channel(Channel *channel)
{
    this->channels.push_back(channel);
}

std::string const  &Client::getCurrentChan()    const   { return (this->current_channel); }
std::string const  &Client::getName()           const   { return (this->nickname); }
std::string const  &Client::getPassword()       const   { return (this->password); }
std::string const  &Client::getUsername()       const   { return (this->username); }
bool                Client::getIsServer()       const   { return (this->is_server); }
bool                Client::getIsPassSet()      const   { return (this->is_pass_set); }
bool                Client::getIsNickSet()      const   { return (this->is_nick_set); }
bool                Client::getIsUserSet()      const   { return (this->is_user_set); }
bool                Client::getIsRegister()     const   { return (this->is_register); }
int                 Client::getFd()             const   { return (this->fd); }


void                Client::setPassSet(bool is_set)                        { this->is_pass_set = is_set; }
void                Client::setNickSet(bool is_set)                        { this->is_nick_set = is_set; }
void                Client::setUserSet(bool is_set)                        { this->is_user_set = is_set; }
void                Client::setRegister(bool is_set)                       { this->is_register = is_set; }

void                Client::setIsServer(bool is_serv)                   { this->is_server = is_serv; }
void				Client::setCurrentChan(std::string const &name)     { this->current_channel = name; }
void                Client::setName(std::string const &name)            { this->nickname = name; }
void                Client::setPassword(std::string const &password)    { this->password = password; }
void                Client::setNickname(std::string const &nickname)    { this->nickname = nickname; }
void                Client::setUsername(std::string const &username)    { this->username = username; }

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