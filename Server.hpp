/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaetan <gaetan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/16 18:48:50 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/19 11:28:28 by gaetan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <arpa/inet.h>
#include <iostream>
#include <sys/time.h>
#include "Client.hpp"
#include "Channel.hpp"

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
		std::list<Channel*> 	channels;
		


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
		fdwrite(client_socket, "Please enter the server password\n");
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
				//check if client input is a command, if it is not, we put what the client said
				//in a string and we resend it to other channels / clients.
				if (buf[0] && buf[0] != '/')
				{
					// simple message case;
					std::string message;
					if ((*it)->getCurrentChan() != "nullptr")
						message = (*it)->getCurrentChan() + ": " + (*it)->getName() + ": " + buf;
					else
						message = (*it)->getName() + ": " + buf;
					std::cout << message << std::endl;
					message = buf;
					clientWriteOnChannel((*it)->getCurrentChan(), message, (*it));
				}
				else
				{
					//command case
					std::string command = buf;
					size_t index = command.find_first_of(' ', 0);
					std::string params = command.substr(index + 1, std::string::npos);
					std::cout << "|" << params << "|" << std::endl;
					command = command.substr(0, index);
					std::cout << "|" << command << "|" << std::endl;
					if (command == "/join")
					{
						//DO the join command
						if (params[0] == '#')
						{
							if (checkChannels(params, (*it)) == 0)
							{
								createChannel(params, (*it));
								std::cout << "Client created a new channel named : " << params << std::endl;
							}
							else
								std::cout << "Client joined channel : " << params << std::endl;
						}
						else
							fdwrite((*it)->getFd(), "Wrong use of JOIN command.\n");
					}
				}
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
		fdwrite(fd_i, "Succesfully connected.\nPlease enter your Nickname: ");
        client->setPass(true);
	}
	else
		fdwrite(client->getFd(), "Wrong password, connection refused.\nPlease try again.\n");
}

void    Server::naming_step(Client *client, int fd_i)
{
    std::string new_name(buf);
    client->setName(new_name);
    client->setStat(true);
    new_name.clear();
    new_name = "Welcome " + client->getName() + "\n";
	fdwrite(fd_i, new_name);
}


void    Server::check_error(int ret, std::string const &str)
{
    if (ret < 0) { std::cerr << str << std::endl; exit(1); }
}


void    Server::resetBuf()
{
	memset(this->buf, 0, 1024);
}

void Server::fdwrite(int fd, std::string str)
{
	write(fd, str.c_str(), str.size());
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

/*
 ___ # _  _  #   _   # _  _  # _  _  # ___  # _     #  ___  #
/ _( #) () ( #  )_\  #) \/ ( #) \/ ( #) __( #) |    # (  _( #
))_  #| -- | # /( )\ #|  \ | #|  \ | #| _)  #| (__  # _) \  #
\__( #)_()_( #)_/ \_(#)_()_( #)_()_( #)___( #)____( #)____) #
     ##       ##       ##       ##       ##      ##       ##       ##

*/

void Server::createChannel(std::string name, Client *client)
{
	Channel *new_chan = new Channel(name);
	channels.push_back(new_chan);
	new_chan->addClient(client);
	client->join_channel(new_chan);
	client->setCurrentChan(new_chan->getChanName());
}

int Server::checkChannels(std::string name, Client *client)
{
	std::list<Channel*>::iterator begin = channels.begin();
	for (std::list<Channel*>::iterator end = channels.end(); begin != end; begin++)
	{
		if (name == (*begin)->getChanName())
		{
			(*begin)->addClient(client);
			client->join_channel(*begin);
			client->setCurrentChan(name);
			clientWriteOnChannel((*begin)->getChanName(), "joined the channel", client);
			return 1;
		}
	}
	return 0;
}

void Server::clientWriteOnChannel(std::string name, std::string msg, Client *client)
{
	std::list<Channel*>::iterator begin = channels.begin();
	for (std::list<Channel*>::iterator end = channels.end(); begin != end; begin++)
	{
		if (name == (*begin)->getChanName())
		{
			std::list<Client*> list = (*begin)->getConnectedClients();
			std::list<Client*>::iterator begin = list.begin();
			for (std::list<Client*>::iterator end = list.end(); begin != end; begin++)
			{
				std::string message = name + ": " + client->getName() + ": " + msg + "\n";
				fdwrite((*begin)->getFd(), message);
			}
			return ;
		}
	}
}

#endif