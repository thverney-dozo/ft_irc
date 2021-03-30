/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 18:48:50 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/30 17:09:24 by thverney         ###   ########.fr       */
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
        struct sockaddr_in      		serv_adr;
        int                     		client_nb;
        char                    		buf[1024];
        char                    	    *host_port;     // host
        char                    	    *local_port;
        std::string             		local_password;
        std::string                     host_ip;        // host
        std::string                     host_password;  // host
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
        Server(const char *local_port, std::string const &local_password, std::string	const &host_ip,
		const char *host_port, std::string host_password);
        virtual ~Server();

        /******************* getters ******************/
        int                     getServSock() const;
        int                     getFdMax() const;
        std::string const       &getPassword() const;
        fd_set                  getReads();
        fd_set                  getCpyReads();
        fd_set                  *getReads_addr();
        fd_set                  *getCpyReads_addr();
        std::vector<Client*>    getClients();
        char                    *getBuf();
        /**********************************************/

        // ########### Setup #############
        void    setup_server(char *port);
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
		void getClientsChannels(Client client); //ca doit retourner une list de channels j'imagine mais la jsuis moi meme perdu dans ce que je fais
        
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