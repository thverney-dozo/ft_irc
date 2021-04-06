/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 02:18:00 by aeoithd           #+#    #+#             */
/*   Updated: 2021/04/06 15:23:17 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() // quand on nous envera pas d'info donc il faudra créer le serveur
{}

Server::Server(std::string const &local_port, std::string const &local_password, std::string const &host_ip,
			   std::string const &host_port, std::string const &host_password)
{
	this->l_port = local_port;
	this->l_password = local_password;

	if (!host_ip.empty() && !host_port.empty())
	{
		this->h_ip = host_ip;
		this->h_port = host_port;
		this->h_password = host_password;
	}

    setup_server(this->l_port.c_str());
	init_commands();
}

Server::~Server()
{}

void     Server::setup_server(std::string const &port)
{
	struct sockaddr_in serv_adr;
    int enable = 1;

	check_error((this->serv_sock = socket(AF_INET, SOCK_STREAM, 0)), "ERROR opening socket") ; 	// Create server's FD
	check_error(setsockopt(this->serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)), "serv_sock can't be used");
	memset(&serv_adr, 0, sizeof(serv_adr));
    
	serv_adr.sin_family = AF_INET; 					// Internet address
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);	// Specify that server will respond to any address
	serv_adr.sin_port = htons(atoi(port.c_str())); 		    // Server will listen to this port
	
	check_error(bind(this->serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)), "bind() error"); // bind
	check_error(listen(this->serv_sock, 5), "listen() error");
    FD_ZERO(&(this->reads));
	FD_SET(this->serv_sock, &(this->reads));
    this->fd_max = this->serv_sock;
	if (!this->h_ip.empty() && !this->h_port.empty())
		setup_host_connexion();
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
		new_client->setConnectionStatus(true);
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
	if (len_buf > 0)
    	this->buf[len_buf - 1] = '\0';
	std::cout << buf << std::endl;
	
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
				std::vector<std::string> splited_cmd = ft_split(buf); // equivalent à un char ** de retour de split
				if (splited_cmd.empty())
					break;
				std::map<std::string, Command>::iterator find_cmd = this->cmd.find(splited_cmd[0]); // Cherche la command
				if (find_cmd != cmd.end())
					(*find_cmd).second.exe(splited_cmd, this, (*it)); // execute la command si elle existe
				else if (find_cmd == cmd.end())
				{
					std::cout << buf << std::endl;
					clientWriteOnChannel((*it)->getCurrentChan(), buf, (*it));
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
	this->cmd.insert(std::pair<std::string, Command>("join", cmd_join));
	this->cmd.insert(std::pair<std::string, Command>("JOIN", cmd_join));
	this->cmd.insert(std::pair<std::string, Command>("/who", cmd_who));
	this->cmd.insert(std::pair<std::string, Command>("who", cmd_who));
	this->cmd.insert(std::pair<std::string, Command>("WHO", cmd_who));
	this->cmd.insert(std::pair<std::string, Command>("/privmsg", cmd_privmsg));
	this->cmd.insert(std::pair<std::string, Command>("privmsg", cmd_privmsg));
	this->cmd.insert(std::pair<std::string, Command>("PRIVMSG", cmd_privmsg));
	this->cmd.insert(std::pair<std::string, Command>("/pass", cmd_pass));
	this->cmd.insert(std::pair<std::string, Command>("pass", cmd_pass));
	this->cmd.insert(std::pair<std::string, Command>("PASS", cmd_pass));
	this->cmd.insert(std::pair<std::string, Command>("/nick", cmd_nick));
	this->cmd.insert(std::pair<std::string, Command>("nick", cmd_nick));
	this->cmd.insert(std::pair<std::string, Command>("NICK", cmd_nick));
	this->cmd.insert(std::pair<std::string, Command>("/user", cmd_user));
	this->cmd.insert(std::pair<std::string, Command>("user", cmd_user));
	this->cmd.insert(std::pair<std::string, Command>("USER", cmd_user));
	this->cmd.insert(std::pair<std::string, Command>("/quit", cmd_quit));
	this->cmd.insert(std::pair<std::string, Command>("quit", cmd_quit));
	this->cmd.insert(std::pair<std::string, Command>("QUIT", cmd_quit));
	this->cmd.insert(std::pair<std::string, Command>("/part", cmd_part));
	this->cmd.insert(std::pair<std::string, Command>("part", cmd_part));
	this->cmd.insert(std::pair<std::string, Command>("PART", cmd_part));
	this->cmd.insert(std::pair<std::string, Command>("/topic", cmd_topic));
	this->cmd.insert(std::pair<std::string, Command>("topic", cmd_topic));
	this->cmd.insert(std::pair<std::string, Command>("TOPIC", cmd_topic));
	this->cmd.insert(std::pair<std::string, Command>("/list", cmd_list));
	this->cmd.insert(std::pair<std::string, Command>("list", cmd_list));
	this->cmd.insert(std::pair<std::string, Command>("LIST", cmd_list));
	
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
	if (splited_cmd.empty()) {
		fdwrite(client->getFd(), "You need to register(/pass, /nick, /user)\n");
		return;
	}
	if (!buf || !buf[0] || splited_cmd[0].empty() || (splited_cmd[0].compare("/pass") != 0 
	&& splited_cmd[0].compare("/nick") != 0 && splited_cmd[0].compare("/user") != 0
	&& splited_cmd[0].compare("nick") && splited_cmd[0].compare("user") != 0
	&& splited_cmd[0].compare("pass") != 0
	&& splited_cmd[0].compare("NICK") && splited_cmd[0].compare("USER") != 0
	&& splited_cmd[0].compare("PASS") != 0))
	{
		fdwrite(client->getFd(), "You need to register with the following input (3 steps):   ");
		if (client->getIsPassSet() == false)
			fdwrite(client->getFd(), "1. \"/pass ...\"\n");
		else if (client->getIsNickSet() == false)
			fdwrite(client->getFd(), "2. \"/nick ...\"\n");
		else if (client->getIsUserSet() == false)
			fdwrite(client->getFd(), "3. \"/user ...\"\n");
	}

	if (client->getIsPassSet() == false && ((splited_cmd[0].compare("/pass") == 0)  || (splited_cmd[0].compare("pass") == 0) || (splited_cmd[0].compare("PASS") == 0)))
	{
        pass_register_step(client, splited_cmd);
		if (client->getIsPassSet() == true) {
			fdwrite(client->getFd(), "You need to register with the following input (3 steps):   ");
			fdwrite(client->getFd(), "2. \"/nick ...\"\n");
		}
	}
	else if (client->getIsNickSet() == false && ((splited_cmd[0].compare("/nick") == 0)  || (splited_cmd[0].compare("nick") == 0) || (splited_cmd[0].compare("NICK") == 0)))
	{    // client needs to input server's password
		nick_register_step(client, splited_cmd);
		if (client->getIsNickSet() == true) {
			fdwrite(client->getFd(), "You need to register with the following input (3 steps):   ");
			fdwrite(client->getFd(), "3. \"/user ...\"\n");
		}
	}
	else if (client->getIsUserSet() == false && ((splited_cmd[0].compare("/user") == 0) || (splited_cmd[0].compare("user") == 0) || (splited_cmd[0].compare("USER") == 0)))	   // client needs to input server's password
        user_register_step(client, splited_cmd);
	if (client->getIsPassSet() == true && client->getIsUserSet() == true && client->getIsNickSet() == true) {
		client->setRegister(true);
		fdwrite(client->getFd(), "Registration done !\n");
	}
}

void	Server::setup_host_connexion()
{
	int connected = 0; // Used to loop the program
 
    std::string nick = "NICK Somebot"; // NICK raw
    std::string user = "USER Somebot randomtext israndom :My first c++ bot"; // USER raw

    struct sockaddr_in addr;
    struct hostent *host; 

	std::cout << "this->h_ip.c_str() = " << this->h_ip.c_str() << std::endl;
    host = gethostbyname(this->h_ip.c_str());
	std::cout << "host = " << host << std::endl;
 
    addr.sin_addr.s_addr = *(unsigned long*)host->h_addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)this->h_int_port);
    int sockd = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << "socket = " << sockd << std::endl;

	if (connect(sockd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		std::cout << "bah nan connard ca marche pas" << std::endl;
 
	if (send(sockd, nick.c_str(), nick.size(), 0) == -1)
		std::cout << "bah nan connard ca marche pas" << std::endl;

	if (send(sockd, user.c_str(), user.size(), 0) == -1) // Converts user string to c-array and sends it to server
		std::cout << "bah nan connard ca marche pas" << std::endl;

	char sockbuff[4096]; // array to hold the incoming socket data
	while (connected < 1)
	{
		memset(&sockbuff, '\0', sizeof(sockbuff)); // make sure sockbuff[] is empty
		recv(sockd, sockbuff, 4096, 0); // Recieve all the data from server to sockbuff[]
		std::cout << sockbuff << std::endl;
		connected = 1;
	}
 
}

void    Server::pass_register_step(Client *client, std::vector<std::string> splited_cmd)
{
	cmd_pass(splited_cmd, this, client);
	if (client->getIsPassSet() == true)
		fdwrite(client->getFd(), "pass step done\n");
	else
		fdwrite(client->getFd(), "failed pass step\n");
	
}

void    Server::nick_register_step(Client *client, std::vector<std::string> splited_cmd)
{
	cmd_nick(splited_cmd, this, client);
	if (client->getIsPassSet() == true)
		fdwrite(client->getFd(), "nick step done\n");
	else
		fdwrite(client->getFd(), "failed nick step\n");
}

void    Server::user_register_step(Client *client, std::vector<std::string> splited_cmd)
{
	cmd_user(splited_cmd, this, client);
	if (client->getIsPassSet() == true)
		fdwrite(client->getFd(), "user step done\n");
	else
		fdwrite(client->getFd(), "failed user step\n");
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

	if (msg.empty())
		return std::vector<std::string>();
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

std::list<Channel*> Server::getChannels() const
{
	return channels;
}

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
std::string const       &Server::getHostIp() const
{
	return (this->h_ip);
}

std::string const       &Server::getHostPort() const
{
	return (this->h_port);
}

std::string const       &Server::getHostPassword() const
{
	return (this->h_password);
}

std::string const       &Server::getLocalPort() const
{
	return (this->l_port);
}

std::string const 		&Server::getLocalPassword() const
{
    return (this->l_password);
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
	fdwrite(client->getFd(), "Channel " + name + " successfully created\n");
}

int Server::checkChannelList(std::string name)
{
	std::list<Channel*>::iterator begin = channels.begin();
	for (std::list<Channel*>::iterator end = channels.end(); begin != end; begin++)
	{
		if(name == (*begin)->getChanName())
			return 1;
	}
	return 0;
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