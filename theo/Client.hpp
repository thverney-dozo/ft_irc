/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeoithd <aeoithd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 22:20:45 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/16 03:42:04 by aeoithd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT
# define CLIENT

#include <arpa/inet.h>
#include <iostream>

class   Client
{
    private:
        int                 fd;
        struct sockaddr_in  addr;
        socklen_t           adr_sz;
        std::string         nickname;
        bool                stat; // client has named himself;

        Client(Client const &cpy);
        Client &operator=(Client const &affect);
    public:
        Client();
        Client(int fd, struct sockaddr_in addr, socklen_t adr_sz);
        virtual             ~Client();
        int                 getFd() const;
        bool                getStat() const;
        std::string const   getName() const;

        void                setName(std::string &name);
        void                setStat(bool is_set);
        void                clear_info();
};

Client::Client() : fd(), addr(), adr_sz(), nickname("noName"), stat(false)
{}

Client::Client(int fd, struct sockaddr_in addr, socklen_t adr_sz) : fd(fd), addr(addr), adr_sz(adr_sz), stat(false)
{}

Client::~Client()
{}

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

std::string const Client::getName() const
{
    return (this->nickname);
}

bool    Client::getStat() const
{
    return (this->stat);
}

void    Client::clear_info()
{
    int                 fd;
        struct sockaddr_in  addr;
        socklen_t           adr_sz;
        std::string         nickname;
        bool                stat; // client has named himself;
}
#endif