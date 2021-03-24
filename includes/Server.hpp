/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeoithd <aeoithd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 18:48:50 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/24 17:50:47 by aeoithd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <arpa/inet.h>
#include <iostream>
#include <sys/time.h>
#include <cstring>
#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include <map>

class Server
{
    private:
        int                     		serv_sock;
        char                    		*port;
        struct sockaddr_in      		serv_adr;
        int                     		client_nb;
        char                    		buf[1024];
        std::string             		password;
        std::vector<Client*>    		clients;
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
        Server(char *port, std::string const &password);
        virtual ~Server();

        // getters
        int                     getServSock() const;
        int                     getFdMax() const;
        std::string const       &getPassword() const;
        fd_set                  getReads();
        fd_set                  getCpyReads();
        fd_set                  *getReads_addr();
        fd_set                  *getCpyReads_addr();
        std::vector<Client*>    getClients();
        char                    *getBuf();

        // Setup
        void    setup_server(char *port);
		void	init_commands();

        
        // Handle server
        int     detection_select();
        void    connexion(); // return number of client   
        void    deconnexion(int fd_i);
        void    receiveFromClient(int fd_i, int buf_len);
		
		//Handle channels
		void createChannel(std::string name, Client *client);
		int checkChannels(std::string name, Client *client);
		void clientWriteOnChannel(std::string name, std::string msg, Client *client);
		void getClientsChannels(Client client); //ca doit retourner une list de channels j'imagine mais la jsuis moi meme perdu dans ce que je fais
        
        // internal server layer
        void    password_step(Client *client, int fd_i);
        void    naming_step(Client *client, int fd_i);

		void	fdwrite(int fd, std::string str);
        void    resetBuf();
        void    check_error(int ret, std::string const &str);
		std::vector<std::string>	ft_split(std::string msg);


        
};

#endif