/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeoithd <aeoithd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 22:20:45 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/24 02:20:51 by aeoithd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT
# define CLIENT

# include <arpa/inet.h>
# include <iostream>
# include <list>

class Channel;

class   Client
{
    private:
        int                 fd;
        struct sockaddr_in  addr;
        socklen_t           adr_sz;
        std::string         nickname;
        bool                stat; // client has named himself;
		bool                pass;
        std::list<Channel*> channels;
		std::string current_channel;


        Client(Client const &cpy);
        Client &operator=(Client const &affect);
    public:
        Client();
        Client(int fd, struct sockaddr_in addr, socklen_t adr_sz);

        void                join_channel(Channel *channel);
        virtual             ~Client();
        int                 getFd() const;
		std::string		getCurrentChan() const;
        bool                getStat() const;
		bool                getPass() const;
        std::string const   &getName() const;

		void				setCurrentChan(std::string name);
        void                setName(std::string &name);
        void                setStat(bool is_set);
		void                setPass(bool is_set);
        void                clear_info();
		int                 getPass(std::string password, int clnt_sock, char tmp[1024]);
};

#include "Channel.hpp"

#endif