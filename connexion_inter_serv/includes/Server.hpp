/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 18:48:50 by aeoithd           #+#    #+#             */
/*   Updated: 2021/04/06 15:03:59 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <arpa/inet.h>
#include <iostream>
#include <sys/time.h>
#include <cstring>
#include <map>
#include <netdb.h>
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"

class Server
{
    private:
        int                     		serv_sock;
        struct sockaddr_in      		serv_adr;
        int                     		client_nb;
        char                    		buf[1024];
        std::string                     l_port;         //local
        std::string             		l_password;     //local
        std::vector<Client*>    		clients;
        fd_set                  		reads;
        fd_set                  		cpy_reads;
        int                     		fd_max;
        struct timeval          		timeout;
		std::list<Channel*> 			channels;
		std::map<std::string, Command>	cmd;
            
        std::string     h_port;     // host
        std::string     h_password; // host
        std::string     h_ip;       // host
        int             h_socket;   // host 
        int             h_int_port; // host
        struct addrinfo	*h_result; // host

        Server(Server const &cpy);
        Server &operator=(Server const &affect);

    public:
        Server();
        Server(std::string const &local_port, std::string const &local_password, std::string const &host_ip,
			   std::string const &host_port, std::string const &host_password);
        virtual ~Server();

        /******************* getters ******************/
        int                     getServSock() const;
        int                     getFdMax() const;
        std::string const       &getHostIp() const;        // host
        std::string const       &getHostPort() const;     // host
        std::string const       &getHostPassword() const;  
        std::string const       &getLocalPort() const;
        std::string const       &getLocalPassword() const;
        fd_set                  getReads();
        fd_set                  getCpyReads();
        fd_set                  *getReads_addr();
        fd_set                  *getCpyReads_addr();
        std::vector<Client*>    getClients();
        char                    *getBuf();
        /**********************************************/

        // ########### Setup #############
        void    setup_server(std::string const &port);
        void	setup_host_connexion();
		void	init_commands();

        
        // ####### Handle server ###########
        int     detection_select();
        void    connexion(); // return number of client   
        void    deconnexion(int fd_i);
        void    receiveFromClient(int fd_i, int buf_len);
		
		// ###### Handle channels #########
		void createChannel(std::string name, Client *client);
		int checkChannels(std::string name, Client *client);
		int checkChannelList(std::string name);
		void clientWriteOnChannel(std::string name, std::string msg, Client *client);
		std::list<Channel*> getChannels() const; //ca doit retourner une list de channels j'imagine mais la jsuis moi meme perdu dans ce que je fais
        
        // ####### internal server layer ##########
        void    registration(Client *client, char *buf);
        void    pass_register_step(Client *client, std::vector<std::string> splited_cmd);
        void    nick_register_step(Client *client, std::vector<std::string> splited_cmd);
        void    user_register_step(Client *client, std::vector<std::string> splited_cmd);

		void	fdwrite(int fd, std::string str);
        void    resetBuf();
        void    check_error(int ret, std::string const &str);
		std::vector<std::string>	ft_split(std::string msg);


        
};

#endif