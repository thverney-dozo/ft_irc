/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 02:18:00 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/28 15:54:12 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() // quand on nous envera pas d'info donc il faudra créer le serveur
{}

Server::Server(char *port, std::string const &password) : port(port), password(password)
{
    setup_server(this->port);
	init_commands();
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
		new_client->setIsServer(false); 									// IMPORTANT à gerer:  valeur par defaul de client 
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
		if (fd_i == (*it)->getFd() && fd_i != serv_sock)
		{
			if ((*it)->getIsRegister() == false)
				registration((*it), buf);
			else
			{
				std::string name = (*it)->getName();
				//check if client input is a command, if it is not, we put what the client said
				//in a string and we resend it to other channels / clients.
				if (buf[0] && buf[0] != '/')
				{
					if ((*it)->getCurrentChan() != "nullptr")
						write((*it)->getFd(), "\033[A\33[2KT\r", 3); // to erase what client wrote on the fd
					std::cout << buf << std::endl;
					clientWriteOnChannel((*it)->getCurrentChan(), buf, (*it));
				}
				else if (buf[0])
				{
					std::vector<std::string> splited_cmd = ft_split(buf); // equivalent à un char ** de retour de split
					std::map<std::string, Command>::iterator find_cmd = this->cmd.find(splited_cmd[0]); // Cherche la command
					if (find_cmd != cmd.end())
						(*find_cmd).second.exe(splited_cmd, this, (*it)); // execute la command si elle existe
				}
				else
				{
					write((*it)->getFd(), "\033[A\33[2KT\r", 3); // to erase what client wrote on the fd
				}
				
			}
            break;
		}
	}
	memset(this->buf, 0, 1024);
}

void	Server::init_commands()
{
	this->cmd.insert(std::pair<std::string, Command>("/join", cmd_join));
	this->cmd.insert(std::pair<std::string, Command>("/who", cmd_who));
	this->cmd.insert(std::pair<std::string, Command>("/privmsg", cmd_privmsg));
	this->cmd.insert(std::pair<std::string, Command>("/pass", cmd_pass));
	this->cmd.insert(std::pair<std::string, Command>("/nick", cmd_nick));
	this->cmd.insert(std::pair<std::string, Command>("/user", cmd_user));
	
	//PASSWORD
	//NICK
	//USER
	//SERVER
	//OPER
	//QUIT
	//SQUIT
	//PART
	//MODE
	//TOPIC
	//NANES
	//LIST
	//INVITE
	//KICK
	//VERSION
	//STATS
	//LINKS
	//CONNECT
	//TRACE
	//ADMIN
	//INFO
	//NOTICE
	//WHOIS
	//WHOWAS
	//KILL
	//PING
	//PONG
	//ERROR
	//SERVICE
	//MOTD
	//LUSERS
	//TIME
	//SERVLIST
	//SQUERY
	//NJOIN
	// soit 37 commandes
}

void    Server::registration(Client *client, char *buf)
{

	std::vector<std::string> splited_cmd = ft_split(buf);
	if (!buf || !buf[0] || splited_cmd[0].empty() || splited_cmd[0].compare("/pass") != 0 || splited_cmd[0].compare("/nick") != 0
	|| splited_cmd[0].compare("/user") != 0)
	{
		fdwrite(client->getFd(), "You need to register with the following input (3 steps):   ");
		if (client->getIsPassSet() == false)
			fdwrite(client->getFd(), "1. \"/pass ...\"\n");
		else if (client->getIsNickSet() == false)
			fdwrite(client->getFd(), "2. \"/nick ...\"\n");
		else if (client->getIsUserSet() == false)
			fdwrite(client->getFd(), "3. \"/user ...\"\n");
	}

	if (client->getIsPassSet() == false && splited_cmd[0].compare("/pass") == 0)     // client needs to input server's password
        pass_register_step(client, splited_cmd);
	else if (client->getIsNickSet() == false && splited_cmd[0].compare("/nick") == 0)     // client needs to input server's password
			nick_register_step(client, splited_cmd);
	else if (client->getIsUserSet() == false && splited_cmd[0].compare("/user") == 0)     // client needs to input server's password
                user_register_step(client, splited_cmd);

	if (client->getIsPassSet() == true && client->getIsUserSet() == true && client->getIsNickSet() == true)
		client->setRegister(true);
}




void    Server::pass_register_step(Client *client, std::vector<std::string> splited_cmd)
{
	std::map<std::string, Command>::iterator find_cmd = this->cmd.find("/pass"); // Cherche la commande
	if (find_cmd != cmd.end())
		(*find_cmd).second.exe(splited_cmd, this, client); // execute la command si elle existe
	
	if (client->getIsPassSet() == true)
		write(client->getFd(), "pass step done\n", 16);
	else
		write(client->getFd(), "failed pass step\n", 16);
	
}

void    Server::nick_register_step(Client *client, std::vector<std::string> splited_cmd)
{
	std::map<std::string, Command>::iterator find_cmd = this->cmd.find("/nick"); // Cherche la commande
	if (find_cmd != cmd.end())
		(*find_cmd).second.exe(splited_cmd, this, client); // execute la command si elle existe

	if (client->getIsPassSet() == true)
		write(client->getFd(), "nick step done\n", 16);
	else
		write(client->getFd(), "failed nick step\n", 16);
}

void    Server::user_register_step(Client *client, std::vector<std::string> splited_cmd)
{
	std::map<std::string, Command>::iterator find_cmd = this->cmd.find("/user"); // Cherche la commande
	if (find_cmd != cmd.end())
		(*find_cmd).second.exe(splited_cmd, this, client); // execute la command si elle existe

	if (client->getIsPassSet() == true)
		write(client->getFd(), "user step done\n", 16);
	else
		write(client->getFd(), "failed user step\n", 16);
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

std::vector<std::string>	Server::ft_split(std::string msg)
{
	std::vector<std::string>	split;
	size_t			            pos = 0;
	std::string		            tmp;
    std::string                 whsp(" \t\f\v\n\r");

	if ((pos = msg.find_last_not_of(whsp)) != std::string::npos)
		msg.erase(pos + 1);
	else
		return (split);
	if ((pos = msg.find_first_not_of(whsp)) != std::string::npos)
		msg = msg.substr(pos);
	else
		return (split);
	while ((pos = msg.find_first_of(whsp)) != std::string::npos)
	{
		tmp = msg.substr(0, pos);
		if (!tmp.empty())
			split.push_back(tmp);
		msg.erase(0, pos + 1);
		if (!msg.empty() && msg[0] == ':')
			break ;
	}
	if (!msg.empty())
		split.push_back(msg);

	if (split.size() > 1 && split.back().size() > 0 && split.back()[0] == ':')
		split.back() = split.back().substr(1);
	return (split);
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