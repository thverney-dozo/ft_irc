/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 02:18:00 by aeoithd           #+#    #+#             */
/*   Updated: 2021/04/14 16:26:40 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() // quand on nous envera pas d'info donc il faudra créer le serveur
{}

Server::Server(std::string const &local_port, std::string const &local_password, std::string const &host_ip,
			   std::string const &host_port, std::string const &host_password)
{
	this->local_port = local_port;
	this->local_password = local_password;

	if (!host_ip.empty() && !host_port.empty())
	{
		this->host_ip = host_ip;
		this->host_port = host_port;
		this->host_password = host_password;
	}
    setup_server(this->local_port.c_str());
	
	if (!host_ip.empty() && !host_port.empty())
		setup_host_connexion();
	
	init_commands();
}

void	Server::setup_host_connexion()
{
	/*
	1. create a socket.
	2. bind* - this is probably be unnecessary because you're the client, not the server.
	3. connect to a server.
	4. send/recv - repeat until we have or receive data
	5. shutdown to end read/write.
	6. close to releases data.
	*/
    int enable = 1;
	struct sockaddr_in host_adr;


	check_error((this->host_sock = socket(AF_INET, SOCK_STREAM, 0)), "ERROR host socket");
	check_error(setsockopt(this->host_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)), "host_sock can't be used");
	host_adr.sin_family = AF_INET;
  	host_adr.sin_port = htons(atoi(this->host_port.c_str()));
  	struct hostent *host_info = gethostbyname(this->host_ip.c_str());
	
	// A REMPLACER PAR UNE FONCTION AUTORISEE !!!!!!!!
	memcpy(&host_adr.sin_addr, host_info->h_addr_list[0], host_info->h_length);
	// A REMPLACER PAR UNE FONCTION AUTORISEE !!!!!!!!
	
	check_error(connect(this->host_sock, (struct sockaddr *)&host_adr, sizeof(host_adr)), "ERROR host connect");
	
	// send(this->host_sock, ":myserv /PASS pass_oui\n", 24, 0);
	send(this->host_sock, ":myserv SERVER test.oulu.fi 1 :[tolsun.oulu.fi]\n:myserv USER thio0247 bahhjsp okbg :theo gaetan\n:myserv NICK thio0247 bahhjsp okbg :theo gaetan\n", 145, 0);
	// fcntl(this->host_sock, F_SETFL, O_NONBLOCK);
	
	Client *new_client = new Client(this->host_sock, host_adr, sizeof(host_adr));
	new_client->setIsServer(true); 
	this->clients.push_back(new_client);
	FD_SET(this->host_sock, &(this->reads));
	if (this->fd_max < this->host_sock)
		this->fd_max = this->host_sock;
	// char *buffer_host;
	// recv(this->host_sock, &buffer_host, 6000, 0);
	// std::cout << "|+0+|" << buffer_host << "|+0+|" << std::endl;
	// send(this->host_sock, "je me coco\n", 12, 0);
	std::cout << "Fin du programme (code 12)" << std::endl;
}

Server::~Server()
{}

void     Server::setup_server(std::string const &port)
{
	struct sockaddr_in serv_adr;
    int enable = 1;

	check_error((this->serv_sock = socket(AF_INET, SOCK_STREAM, 0)), "ERROR opening socket"); 	// Create server's FD
	check_error(setsockopt(this->serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)), "serv_sock can't be used");
	memset(&serv_adr, 0, sizeof(serv_adr));
    
	serv_adr.sin_family = AF_INET; 					// Internet address
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);	// Specify that server will respond to any address
	serv_adr.sin_port = htons(atoi(port.c_str())); 	// Server will listen to this port
	
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
	// if (ret)
	// 	std::cout << "Select triggered " << ret << std::endl;
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
				std::map<std::string, Command>::iterator find_cmd;
				if (splited_cmd[0][0] == ':' && !splited_cmd[1].empty()) // si un serveur envoie des infos ":nomDuServ /CMD ..."
					find_cmd = this->cmd.find(splited_cmd[1]); // Cherche la command extra-server
				else
					find_cmd = this->cmd.find(splited_cmd[0]); // Cherche la command intra-server
				if (find_cmd != cmd.end())
					(*find_cmd).second.exe(splited_cmd, this, (*it)); // execute la command si elle existe
				else if (find_cmd == cmd.end())
				{
					std::cout << "Bon bah ca marche plus" << std::endl;
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
	int i = 0;
	for (size_t j = 0; j != splited_cmd.size(); j++)
	{
		//Check si la commande envoyee par le client est user || nick || cap ls pr recupererer les infos de capabilities du server
		//Si c'est aucun de ceux la, renvoie une erreur
		std::vector<std::string> splited_cmd = ft_split(buf); // equivalent à un char ** de retour de split
		if (splited_cmd.empty())
			break;
		if (splited_cmd[j].compare("CAP") == 0 || splited_cmd[j].compare("LS") == 0)
			i++;
		if (splited_cmd[j].compare("user") != 0 && splited_cmd[j].compare("USER") != 0 && splited_cmd[j].compare("/user") != 0 && splited_cmd[j].compare("NICK") != 0 &&
			splited_cmd[j].compare("nick") != 0 && splited_cmd[j].compare("/nick") != 0)
			continue;
		std::map<std::string, Command>::iterator find_cmd = this->cmd.find(splited_cmd[j]); // Cherche la command
		if (find_cmd != cmd.end())
		{
			i++;
			std::vector<std::string> cmds;
			cmds.push_back(splited_cmd[j]);
			if (splited_cmd[j].compare("NICK") == 0 || splited_cmd[j].compare("nick") == 0 || splited_cmd[j].compare("/nick") == 0)
				cmds.push_back(splited_cmd[j + 1]);
			if (splited_cmd[j].compare("USER") == 0 || splited_cmd[j].compare("user") == 0 || splited_cmd[j].compare("/user") == 0)
				{
					while (j != splited_cmd.size() - 1)
						cmds.push_back(splited_cmd[++j]);
				}
			(*find_cmd).second.exe(cmds, this, client); // execute la command si elle existe
		}
	}
	if (client->getIsNickSet() == true && client->getIsUserSet() == true)
		client->setRegister(true);
	else if (i == 0)
		fdwrite(client->getFd(), "You need to register before anything else.\nTry those commands:\n-USER\n-NICK\n");
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
	return (this->host_ip);
}

std::string const       &Server::getHostPort() const
{
	return (this->host_port);
}

std::string const       &Server::getHostPassword() const
{
	return (this->host_password);
}

std::string const       &Server::getLocalPort() const
{
	return (this->local_port);
}

std::string const 		&Server::getLocalPassword() const
{
    return (this->local_password);
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