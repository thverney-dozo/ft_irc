/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 22:20:45 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/28 15:40:13 by thverney         ###   ########.fr       */
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
        bool                is_server; // server or client;
    
        /************** channels ************/
        std::list<Channel*> channels;
		std::string         current_channel;
        /************************************/


        /******** client info ********/
        std::string         nickname;
        std::string         password;
        std::string         username;
        /*****************************/


        /********** registration ********/
		bool                is_pass_set;
		bool                is_nick_set;
		bool                is_user_set;
        bool                is_register;
        /********************************/

        Client(Client const &cpy);
        Client &operator=(Client const &affect);

    public:
    
        Client();
        Client(int fd, struct sockaddr_in addr, socklen_t adr_sz);
        virtual             ~Client();

        void                join_channel(Channel *channel);

        // **************************** getters **********************************
        int                 getFd()                 const;
        bool                getIsPassSet()          const;
        bool                getIsNickSet()          const;
        bool                getIsUserSet()          const;
        bool                getIsRegister()         const;
        bool                getIsServer()           const;
        std::string const   &getName()              const;
        std::string const   &getPassword()          const;
        std::string const   &getUsername()          const;
        std::string const   &getNickname()          const;
		std::string const   &getCurrentChan()       const;
		int                 getPass(std::string password, int clnt_sock, char tmp[1024]);
        // ***********************************************************************

        // **************************** setters **********************************
		void				setCurrentChan(std::string const &name);
        void                setName(std::string const &name);
        void                setPassword(std::string const &password);
        void                setUsername(std::string const &usename);
        void                setNickname(std::string const &nickname);
        void                setPassSet(bool is_set);
        void                setNickSet(bool is_set);
        void                setUserSet(bool is_set);
        void                setRegister(bool is_register);
        void                setIsServer(bool is_serv);
        // ***********************************************************************
        void                clear_info();
};

#include "Channel.hpp"

#endif