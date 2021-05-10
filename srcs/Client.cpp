/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaetan <gaetan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 02:19:44 by aeoithd           #+#    #+#             */
/*   Updated: 2021/05/10 16:41:38 by gaetan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : fd(), addr(), adr_sz(), current_channel("nullptr")
{
    this->is_pass_set = false;
    this->is_user_set = false;
    this->is_nick_set = false;
    this->is_register = false;
	this->client_buf = "";
}

Client::Client(int fd, struct sockaddr_in addr, socklen_t adr_sz) : fd(fd), addr(addr), adr_sz(adr_sz), current_channel("nullptr")
{
    this->is_pass_set = false;
    this->is_user_set = false;
    this->is_nick_set = false;
    this->is_register = false;
	this->client_buf = "";
}

Client::~Client()
{}

void                Client::join_channel(Channel *channel)
{
    this->channels.push_back(channel);
}

void				Client::leave_channel(std::string name)
{
	std::list<Channel*>::iterator begin = channels.begin();
	for (std::list<Channel*>::iterator end = channels.end(); begin != end; begin++)
	{
		if ((*begin)->getChanName() == name)
		{
			(*begin)->userLeftChan(this);
			channels.erase(begin);
			return ;
		}
	}
}

std::string const &Client::getCurrentChan()    	const   { return (this->current_channel); }
std::string const &Client::getName()           	const   { return (this->nickname); }
std::string const &Client::getPassword()       	const   { return (this->password); }
std::string const &Client::getUsername()       	const   { return (this->username); }
std::string const &Client::getClientBuf()     	const   { return (this->client_buf); }
std::string const &Client::getServName() 		const { return server_name; }
std::string const &Client::getHostName() 		const { return host_name; }
std::string const &Client::getRealName() 		const { return real_name; }
bool				Client::getIsHostName() const { return host_name_set; }
bool				Client::getIsServName() const { return serv_name_set; }
bool				Client::getIsRealName() const { return real_name_set; }
bool                Client::getIsServer()       const   { return (this->is_server); }
bool                Client::getIsPassSet()      const   { return (this->is_pass_set); }
bool                Client::getIsNickSet()      const   { return (this->is_nick_set); }
bool                Client::getIsUserSet()      const   { return (this->is_user_set); }
bool                Client::getIsRegister()     const   { return (this->is_register); }
bool				Client::getConnectionStatus() const { return (this->is_connected); }
bool				Client::getIsOp() {return is_op;}
int                 Client::getFd()             const   { return (this->fd); }


void                Client::setPassSet(bool is_set)                        { this->is_pass_set = is_set; }
void                Client::setNickSet(bool is_set)                        { this->is_nick_set = is_set; }
void                Client::setUserSet(bool is_set)                        { this->is_user_set = is_set; }
void                Client::setRegister(bool is_set)                       { this->is_register = is_set; }
void                Client::setHostName(bool is_set)                       { this->host_name_set = is_set; }
void                Client::setServerName(bool is_set)                       { this->serv_name_set = is_set; }
void                Client::setRealName(bool is_set)                       { this->real_name_set = is_set; }
void				Client::setOp(bool isop)										{is_op = isop;}

void                Client::setIsServer(bool is_serv)                   { this->is_server = is_serv; }
void				Client::setHostName(std::string const &hostname) 	{ host_name = hostname; }
void				Client::setServerName(std::string const &servername) { server_name = servername; }
void				Client::setRealName(std::string const &realname) 	{ real_name = realname; }
void				Client::setCurrentChan(std::string const &name)     { this->current_channel = name; }
void                Client::setName(std::string const &name)            { this->nickname = name; }
void                Client::setPassword(std::string const &password)    { this->password = password; }
void                Client::setNickname(std::string const &nickname)    { this->nickname = nickname; }
void                Client::setUsername(std::string const &username)    { this->username = username; }
void				Client::setConnectionStatus(bool is_connected) 		{ this->is_connected = is_connected; }
void				Client::addToClientBuffer(const char *buf) 			{ this->client_buf += buf; }
void				Client::clearClientBuf() 							{ this->client_buf.clear(); }


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

void Client::write_on_all_chans(std::string msg)
{
	std::list<Channel*>::iterator begin = channels.begin();
	for (std::list<Channel*>::iterator end = channels.end(); begin != end; begin++)
		(*begin)->clientWriteMsg(msg, this);
}

std::string const& Client::getMods() const
{
	return mods;
}

void Client::addMod(char mod)
{
	size_t pos =mods.find(mod);
	if (pos == std::string::npos)
		mods += mod;
}

void Client::remMod(char mod)
{
	size_t pos = mods.find(mod);
	if (pos != std::string::npos)
		mods.erase(pos, 1);
}