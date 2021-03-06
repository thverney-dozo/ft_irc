/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaetan <gaetan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 22:20:45 by aeoithd           #+#    #+#             */
/*   Updated: 2021/06/29 12:11:22 by gaetan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT
# define CLIENT

# include <arpa/inet.h>
# include <iostream>
# include <list>
# include <vector>

class Channel;

class   Client
{
    private:
        int                 fd;
        struct sockaddr_in  addr;
        socklen_t           adr_sz;
        bool                is_server; // server or client;
        std::string         client_buf;
		
        /************** channels ************/
        std::list<Channel*> channels;
		std::string         current_channel;
		std::vector<std::string> invitelist;
        /************************************/


        /******** client info ********/
        std::string         nickname;
        std::string         password;
        std::string         username;
		std::string			real_name;
		std::string			host_name;
		std::string			server_name;
		std::string			mods;
		bool				  is_connected;
		bool				 is_op;
        /*****************************/


        /********** registration ********/
		bool                is_pass_set;
		bool                is_nick_set;
		bool                is_user_set;
        bool                is_register;
		bool				serv_name_set;
		bool				host_name_set;
		bool				real_name_set;
        /********************************/

        Client(Client const &cpy);
        Client &operator=(Client const &affect);

    public:
    
        Client();
        Client(int fd, struct sockaddr_in addr, socklen_t adr_sz);
        virtual             ~Client();

        void                join_channel(Channel *channel);
		void				leave_channel(std::string name);
		void				write_on_all_chans(std::string msg);

        // **************************** getters **********************************
        int                 getFd()                 const;
        bool                getIsPassSet()          const;
        bool                getIsNickSet()          const;
        bool                getIsUserSet()          const;
        bool                getIsRegister()         const;
        bool                getIsServer()           const;
		bool				getConnectionStatus() const;
		bool				getIsHostName() const;
		bool				getIsServName() const;
		bool				getIsRealName() const;
        std::string const   &getClientBuf()              const;
        std::string const   &getName()              const;
        std::string const   &getPassword()          const;
        std::string const   &getUsername()          const;
        std::string const   &getNickname()          const;
		std::string const   &getCurrentChan()       const;
		std::string const &getRealName() const;
		std::string const &getServName() const;
		std::string const &getHostName() const;
		std::string const &getMods() const;
		bool					getIsOp();
		int                 getPass(std::string password, int clnt_sock, char tmp[1024]);
		std::list<Channel*> getChanList();
		bool getInvite(std::string name);
        // ***********************************************************************

        // **************************** setters **********************************
		void				removeInvite(std::string name);
		void				setCurrentChan(std::string const &name);
		void				addToClientBuffer(const char *buf);
        void				clearClientBuf();
		void				setOp(bool isop);
        void                setName(std::string const &name);
        void                setPassword(std::string const &password);
        void                setUsername(std::string const &usename);
        void                setNickname(std::string const &nickname);
		void				setHostName(std::string const &hostname);
		void				setServerName(std::string const &servername);
		void				setRealName(std::string const &realname);
		void				setHostName(bool is_set);
		void				setServerName(bool is_set);
		void				setRealName(bool is_set);
        void                setPassSet(bool is_set);
        void                setNickSet(bool is_set);
        void                setUserSet(bool is_set);
        void                setRegister(bool is_register);
        void                setIsServer(bool is_serv);
		void				setConnectionStatus(bool is_connected);
		void				addMod(char mod);
		void				remMod(char mod);
		void				addInvite(std::string name);
        // ***********************************************************************
        void                clear_info();
		int CheckChannels(std::string name);
};

#include "Channel.hpp"

#endif