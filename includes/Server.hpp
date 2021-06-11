/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 18:48:50 by aeoithd           #+#    #+#             */
/*   Updated: 2021/06/11 07:22:55 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <sys/time.h>
#include <fcntl.h>
#include <locale>
#include <algorithm>
#include <cstring>
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include <map>

class Server
{
    private:
        int                     		serv_sock;
        int                     		host_sock;
        struct sockaddr_in      		serv_adr;
        int                     		client_nb;
        std::string                     host_port;     // host
        std::string                     local_port;
        std::string             		local_password;
        std::string                     host_ip;        // host
        std::string                     host_password;  // host
        std::map<int, Client*>          clients;
        fd_set                  		reads;
        fd_set                  		cpy_reads;
        int                     		fd_max;
        struct timeval          		timeout;
		std::list<Channel*> 			channels;
		std::map<std::string, Command>	cmd;


        Server(Server const &cpy);
        Server &operator=(Server const &affect);

    public:
        Server();
        Server(std::string const &local_port, std::string const &local_password, std::string const &host_ip,
			   std::string const &host_port, std::string const &host_password);
        virtual ~Server();
        Client                          *Sender;   // current client to deal with
		std::map<int, std::string>	    clients_buffer;

        /******************* getters ******************/
        int                     getServSock() const;
        int                     getHostSock() const;
        int                     getFdMax() const;
		Channel			        *getThisChan(std::string name);
        std::string const       &getBuf() const;
        std::string const       &getHostIp() const;        // host
        std::string const       &getHostPort() const;     // host
        std::string const       &getHostPassword() const;  
        std::string const       &getLocalPort() const;
        std::string const       &getLocalPassword() const;
        fd_set                  getReads();
        fd_set                  getCpyReads();
        fd_set                  *getReads_addr();
        fd_set                  *getCpyReads_addr();
        std::map<int, Client*>  getClients();
        /**********************************************/


        // ########### Setup #############
        void    setup_server(std::string const &port);
        void	setup_host_connexion();
		void	init_commands();

        
        // ####### Handle server ###########
        int     detection_select();
        void    connexion(); // return number of client   
        void    deconnexion(int fd_i);
        int     find_cmd(std::string data, Client *Sender);
        void	send_data_to_network(std::vector<std::string>::iterator data_cursor);
        void    receiveFromClient(int fd_i);
		
		// ###### Handle channels #########
		void createChannel(std::string name, Client *client);
		int checkChannels(std::string name, Client *client);
		int checkChannelList(std::string name);
		void clientWriteOnChannel(std::string name, std::string msg, Client *client);
		std::list<Channel*> getChannels() const; //ca doit retourner une list de channels j'imagine mais la jsuis moi meme perdu dans ce que je fais
        
        // ####### internal server layer ##########
        void    registration(Client *client, const char *buf);
        void    pass_register_step(Client *client, std::vector<std::string> splited_cmd);
        void    nick_register_step(Client *client, std::vector<std::string> splited_cmd);
        void    user_register_step(Client *client, std::vector<std::string> splited_cmd);

		void	fdwrite(int fd, std::string str);
        void    check_error(int ret, std::string const &str);
		std::vector<std::string>	ft_split_cmd(std::string msg);
        std::vector<std::string>	ft_split_recv_data(std::string data);

        void    seg(int i1, int i2 = 0, int i3 = 0, int i4 = 0)
        {
            std::cout << "~~~ < seg code > "<< i1;
            if (i2)
                std::cout << "_._" << i2;
            if (i3)
                std::cout << "_._" << i3;
            if (i4)
                std::cout << "_._" << i4;
            std::cout << std::endl;
        }        
};

#endif