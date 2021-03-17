/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeoithd <aeoithd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 18:48:50 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/17 01:21:23 by aeoithd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <arpa/inet.h>
#include <iostream>
#include <sys/time.h>
#include "Client.hpp"


class Server
{
    private:
        int                     serv_sock;
        char                    *port;
        struct sockaddr_in      serv_adr;
        int                     client_nb;
        char                    buf[1024];
        std::string             password;
        std::vector<Client*>    clients;
        fd_set                  reads;
        fd_set                  cpy_reads;
        int                     fd_max;
        struct timeval          timeout;


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
        
        // Handle server
        int     detection_select();
        void    connexion(); // return number of client   
        void    deconnexion(int fd_i);
        void    receiveFromClient(int fd_i, int buf_len);
        
        // internal server layer
        void    password_step(Client *client, int fd_i);
        void    naming_step(Client *client, int fd_i);


        void    resetBuf();
        void    check_error(int ret, std::string const &str);


        
};

Server::Server() // quand on nous envera pas d'info donc il faudra créer le serveur
{}

Server::Server(char *port, std::string const &password) : port(port), password(password)
{
    setup_server(this->port);
}

Server::~Server()
{}

void     Server::setup_server(char *port)
{
	struct sockaddr_in serv_adr;
    int enable = 1;

	check_error((this->serv_sock = socket(AF_INET, SOCK_STREAM, 0)), "ERROR opening socket") ; 	// Create server's FD
	check_error(setsockopt(this->serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)), "serv_sock can't be used");
	memset(&serv_adr, 0, sizeof(serv_adr));
    
	serv_adr.sin_family = AF_INET; 					// Internet address
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);	// Specify that server will respond to any address
	serv_adr.sin_port = htons(atoi(port)); 		    // Server will listen to this port
	
	check_error(bind(this->serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)), "bind() error"); // bind
	check_error(listen(this->serv_sock, 5), "listen() error");
    FD_ZERO(&(this->reads));
	FD_SET(this->serv_sock, &(this->reads));
    this->fd_max = this->serv_sock;
}

int     Server::detection_select()
{
    int ret;

    this->cpy_reads = this->reads;
    this->timeout.tv_sec = 5;
	this->timeout.tv_usec = 5000;
    ret = select(this->fd_max + 1, &(this->cpy_reads), 0 , 0, &(this->timeout));

    return ret;
}

void     Server::connexion()
{
    int client_socket;
    struct sockaddr_in clnt_adr;
	socklen_t adr_sz = sizeof(clnt_adr);

    client_socket = accept(this->serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
    
    if (client_socket != -1)
	{
		Client *new_client = new Client(client_socket, clnt_adr, adr_sz);
		clients.push_back(new_client);
		FD_SET(client_socket, &(this->reads));
		std::cout << "[Client connected]" << std::endl;
		write(client_socket, "Please enter the server password\n", 34);
		if (this->fd_max < client_socket)
			this->fd_max = client_socket;
	}
}

void    Server::deconnexion(int fd_i)
{
    FD_CLR(fd_i, &reads);
	close(fd_i);
	std::cout << "[Client disconnected] (";
	std::vector<Client*>::iterator ite = clients.end();
	for (std::vector<Client*>::iterator it = clients.begin(); it != ite; ++it) // find my client
	{
		if (fd_i == (*it)->getFd()) {
			std::cout << (*it)->getName() << ")" << std::endl;
			clients.erase(it);
			break;
		}
	}
}

void    Server::receiveFromClient(int fd_i, int len_buf)
{
    this->buf[len_buf - 1] = '\0';

    std::vector<Client*>::iterator ite = this->clients.end();
    for (std::vector<Client*>::iterator it = this->clients.begin(); it != ite; ++it) // find my client
	{
		if (fd_i == (*it)->getFd() && fd_i != serv_sock) {
			if ((*it)->getPass() == false)     // client needs to input server's password
                password_step((*it), fd_i);
			else if ((*it)->getStat() == false) // has my client named himself
                naming_step((*it), fd_i);
			else                //need to check commands with a else if
			{
				std::string name = (*it)->getName();
				write(1, name.c_str(), name.size());
				write(1, ": ", 2);
				write(1, this->buf, len_buf);
				write(1, "\n", 1);
			}
            break;
		}
	}
	memset(this->buf, 0, 1024);
}

void    Server::password_step(Client *client, int fd_i)
{
    if (client->getPass(this->password, client->getFd(), this->buf) == 1)
	{
        write(client->getFd(), "Succesfully connected\n", 23);
        client->setPass(true);
        write(fd_i, "Please enter Nickname: ", 24);
	}
	else
	{
        write(client->getFd(), "Wrong password, connection refused.\n", 37);
		write(client->getFd(), "Please try again.\n", 19);
	}
}

void    Server::naming_step(Client *client, int fd_i)
{
    std::string new_name(buf);
    client->setName(new_name);
    client->setStat(true);
    new_name.clear();
    new_name = client->getName();
    write(fd_i, "Welcome ", 9);
    write(fd_i, new_name.c_str(), new_name.size());
    write(fd_i, "\n", 1);
}


void    Server::check_error(int ret, std::string const &str)
{
    if (ret < 0) { std::cerr << str << std::endl; exit(1); }
}


void    Server::resetBuf()
{
	memset(this->buf, 0, 1024);
}

/*          _   _                
           | | | |               
  __ _  ___| |_| |_ ___ _ __ ___ 
 / _` |/ _ \ __| __/ _ \ '__/ __|
| (_| |  __/ |_| ||  __/ |  \__ \
 \__, |\___|\__|\__\___|_|  |___/
  __/ |                          
 |___/                           
*/

int     Server::getServSock() const
{
    return (this->serv_sock);
}

int     Server::getFdMax() const
{
    return (this->fd_max);
}

char     *Server::getBuf()
{
    return (this->buf);
}

std::string const &Server::getPassword() const
{
    return (this->password);
}

fd_set      Server::getReads()
{
    return (this->reads);
}

fd_set      Server::getCpyReads()
{
    return (this->cpy_reads);
}

fd_set      *Server::getReads_addr()
{
    return (&(this->reads));
}

fd_set      *Server::getCpyReads_addr()
{
    return (&(this->cpy_reads));
}

std::vector<Client*>    Server::getClients()
{
    return (this->clients);
}


#endif