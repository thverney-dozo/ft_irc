/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 02:18:00 by aeoithd           #+#    #+#             */
/*   Updated: 2021/07/02 14:29:46 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() // quand on nous envera pas d'info donc il faudra créer le serveur
{}

Server::Server(std::string const &local_port, std::string const &local_password)
{
	this->local_port = local_port;
	this->local_password = local_password;

    setup_server(this->local_port.c_str());
	
	init_commands();
}

Server::~Server()
{}

void     Server::setup_server(std::string const &port)
{
	struct sockaddr_in serv_adr;
	int enable = -1;

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
    this->timeout.tv_sec = 1;
	this->timeout.tv_usec = 1000;
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
		fcntl(client_socket, F_SETFL, O_NONBLOCK);
		Client *new_client = new Client(client_socket, clnt_adr, adr_sz);
		new_client->setIsServer(false); 									// IMPORTANT à gerer:  valeur par defaul de client 
		this->clients.insert(std::pair<int, Client*>(client_socket, new_client));
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
	std::cout << this->clients.find(fd_i)->second->getName() << ")" << std::endl;
	this->clients.erase(fd_i);
}

int		Server::find_cmd(std::string data, Client *Sender)
{
	std::vector<std::string> splited_cmd = ft_split_cmd(data);

	if (!splited_cmd.empty() && !splited_cmd[0].empty() && this->cmd.find(splited_cmd[0]) != this->cmd.end())
		this->cmd.find(splited_cmd[0])->second.exe(splited_cmd, this, Sender); // execute la command si elle existe
	else
		return (0);
	return (1);
}

void	Server::send_data_to_network(std::vector<std::string>::iterator data_cursor)
{
	std::map<int, Client*>::iterator ite_serv = this->clients.end();
	
	for (std::map<int, Client*>::iterator it_serv = this->clients.begin(); it_serv != ite_serv; ++it_serv)
		if ((*it_serv).second->getIsServer() == true)
			fdwrite((*it_serv).second->getFd(), (*data_cursor+ "\n").c_str());
}

void    Server::receiveFromClient(int fd_i)
{

	// std::cout << this->clients_buffer[fd_i];

	if (Sender->getIsServer() == true)
		return;
	std::vector<std::string> splited_by_line_recv_data = ft_split_recv_data(this->clients_buffer[fd_i]);
	std::vector<std::string>::iterator data_end = splited_by_line_recv_data.end();
	for (std::vector<std::string>::iterator data_cursor = splited_by_line_recv_data.begin(); data_cursor != data_end; ++data_cursor)
	{
		std::string dat = (*data_cursor);
		// dat.erase(dat.size() - 1, 1);
		std::cout << "\n->data to process: " << dat << std::endl;
		if (Sender->getIsRegister() == false)
				registration_client(Sender, (*data_cursor)); // registration(Sender, (*data_cursor).c_str());
		else if (find_cmd((*data_cursor), Sender)) // return if cmd was found and launched
				return;
		else
		{
			send_data_to_network(data_cursor);
			clientWriteOnChannel(Sender->getCurrentChan(), (*data_cursor), Sender);
		}
	}
}

void	Server::init_commands()
{
	this->cmd.insert(std::pair<std::string, Command>("/join", cmd_join));
	this->cmd.insert(std::pair<std::string, Command>("join", cmd_join));
	this->cmd.insert(std::pair<std::string, Command>("JOIN", cmd_join));
	// this->cmd.insert(std::pair<std::string, Command>("/who", cmd_who));
	// this->cmd.insert(std::pair<std::string, Command>("who", cmd_who));
	// this->cmd.insert(std::pair<std::string, Command>("WHO", cmd_who));
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
	// this->cmd.insert(std::pair<std::string, Command>("/topic", cmd_topic));
	// this->cmd.insert(std::pair<std::string, Command>("topic", cmd_topic));
	// this->cmd.insert(std::pair<std::string, Command>("TOPIC", cmd_topic));
	this->cmd.insert(std::pair<std::string, Command>("/list", cmd_list));
	this->cmd.insert(std::pair<std::string, Command>("list", cmd_list));
	this->cmd.insert(std::pair<std::string, Command>("LIST", cmd_list));
	 this->cmd.insert(std::pair<std::string, Command>("/mode", cmd_mode));
	 this->cmd.insert(std::pair<std::string, Command>("mode", cmd_mode));
	 this->cmd.insert(std::pair<std::string, Command>("MODE", cmd_mode));
	 this->cmd.insert(std::pair<std::string, Command>("kick", cmd_kick));
	 this->cmd.insert(std::pair<std::string, Command>("KICK", cmd_kick));
	 this->cmd.insert(std::pair<std::string, Command>("/kick", cmd_kick));
	 this->cmd.insert(std::pair<std::string, Command>("invite", cmd_invite));
	 this->cmd.insert(std::pair<std::string, Command>("/invite", cmd_invite));
	 this->cmd.insert(std::pair<std::string, Command>("INVITE", cmd_invite));
	
	// PASS
	
	// NICK
	// USER
	// SERVER
	// QUIT
	// PART
	// TOPIC
	// PRIVMSG
	// LIST
	// WHO

	// OPER
	// SQUIT
	// MODE
	// NANES
	// INVITE
	// KICK
	// VERSION
	// STATS
	// LINKS
	// CONNECT
	// TRACE
	// ADMIN
	// INFO
	// NOTICE
	// WHOIS
	// WHOWAS
	// KILL
	// PING
	// PONG
	// ERROR
	// SERVICE
	// MOTD
	// LUSERS
	// TIME
	// SERVLIST
	// SQUERY
	// NJOIN
	//  soit 37 commandes
}

void    Server::registration_client(Client *client, std::string const &s)
{
	if (!local_password.empty() && (s.compare(0,4,"PASS") == 0 || s.compare(0,5,"/pass") == 0 || s.compare(0,4,"pass") == 0))
		find_cmd(s, Sender);
	if (!local_password.empty() && client->getIsPassSet() == false)
		return;
	// 	deconnexion(client->getFd());
	if (s.compare(0,4,"NICK") == 0 || s.compare(0,4,"nick") == 0 || s.compare(0,5,"/nick") == 0
	|| s.compare(0,4,"USER") == 0 || s.compare(0,4,"user") == 0 || s.compare(0,5,"/user") == 0)
		find_cmd(s, Sender);
	if (client->getIsNickSet() == true && client->getIsUserSet() == true)
		client->setRegister(true);
}

// void    Server::registration(Client *client, const char *buf)
// {
// 	std::vector<std::string> splited_cmd = ft_split_cmd(buf);
// 	int i = 0;
// 	for (size_t j = 0; j != splited_cmd.size(); j++)
// 	{
// 		//Check si la commande envoyee par le client est user || nick || cap ls pr recupererer les infos de capabilities du server
// 		//Si c'est aucun de ceux la, renvoie une erreur
// 		std::vector<std::string> splited_cmd = ft_split_cmd(buf); // equivalent à un char ** de retour de split
// 		if (splited_cmd.empty())
// 			break;
// 		if (splited_cmd[0][0] == ':' && !splited_cmd[1].empty())
// 			j++;
// 		if (splited_cmd[j].compare("CAP") == 0 || splited_cmd[j].compare("LS") == 0)
// 			i++;
// 		if (splited_cmd[j].compare("user") != 0 && splited_cmd[j].compare("USER") != 0 && splited_cmd[j].compare("/user") != 0 && splited_cmd[j].compare("NICK") != 0 &&
// 			splited_cmd[j].compare("nick") != 0 && splited_cmd[j].compare("/nick") != 0)
// 			continue;
// 		std::map<std::string, Command>::iterator find_cmd = this->cmd.find(splited_cmd[j]); // Cherche la command
// 		if (find_cmd != cmd.end())
// 		{
// 			i++;
// 			std::vector<std::string> cmds;
// 			cmds.push_back(splited_cmd[j]);
// 			if (splited_cmd[j].compare("NICK") == 0 || splited_cmd[j].compare("nick") == 0 || splited_cmd[j].compare("/nick") == 0)
// 				cmds.push_back(splited_cmd[j + 1]);
// 			if (splited_cmd[j].compare("USER") == 0 || splited_cmd[j].compare("user") == 0 || splited_cmd[j].compare("/user") == 0)
// 				{
// 					while (j != splited_cmd.size() - 1)
// 						cmds.push_back(splited_cmd[++j]);
// 				}
// 			(*find_cmd).second.exe(cmds, this, client); // execute la command si elle existe
// 		}
// 	}
// 	if (client->getIsNickSet() == true && client->getIsUserSet() == true)
// 		client->setRegister(true);
// 	if (client->getIsRegister() == true)
// 		fdwrite(client->getFd(), "Registration done !\n");
// }

void    Server::check_error(int ret, std::string const &str)
{
    if (ret < 0) { std::cerr << str << std::endl; exit(1); }
}

void Server::fdwrite(int fd, std::string str)
{
	write(fd, str.c_str(), str.size());
}

std::vector<std::string>	Server::ft_split_cmd(std::string msg)
{
	std::vector<std::string>	split;
	size_t			            pos = 0;
	std::string		            tmp;
    std::string                 whsp(" \t\f\v\n\r");

	if (msg[0] == ' ')
		return std::vector<std::string>();
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

std::vector<std::string>	Server::ft_split_recv_data(std::string data)
{
	std::vector<std::string>	split;
	size_t			            pos = 0;
	std::string		            tmp;

	if (data.empty())
		return std::vector<std::string>();
	while ((pos = data.find_first_of("\n")) != std::string::npos)
	{
		// std::cout << "{+++++++}" << std::endl;
		tmp = data.substr(0, pos);
		if (!tmp.empty())
			split.push_back(tmp);
		data.erase(0, pos + 1);
	}
	if (!data.empty())
		split.push_back(data);
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

int     Server::getHostSock() const
{
    return (this->host_sock);
}

int     Server::getFdMax() const
{
    return (this->fd_max);
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

std::map<int, Client*>    Server::getClients()
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
	new_chan->addPremiumClient(client);
	// client->setCurrentChan(new_chan->getChanName());
	fdwrite(client->getFd(), ":" + client->getName() + "!localhost JOIN " + name + "\r\n");
	// fdwrite(client->getFd(), "Channel " + name + " successfully created\n");
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

Channel *Server::getThisChan(std::string name)
{
	std::list<Channel*>::iterator begin = channels.begin();
	for (std::list<Channel*>::iterator end = channels.end(); begin != end; begin++)
	{
		if(name == (*begin)->getChanName())
			return (*begin);
	}
	return NULL;
}

int Server::checkChannels(std::string name, Client *client)
{
	(void)client;
	std::list<Channel*>::iterator begin = channels.begin();
	for (std::list<Channel*>::iterator end = channels.end(); begin != end; begin++)
	{
		if (name == (*begin)->getChanName())
			return 1;
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