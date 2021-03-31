/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaetan <gaetan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 19:21:49 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/30 11:33:41 by gaetan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Client.hpp"
# include <vector>


class Server;

void	cmd_join(std::vector<std::string> split, Server *serv, Client *client);
void	cmd_who(std::vector<std::string> split, Server *serv, Client *client);
void	cmd_privmsg(std::vector<std::string> split, Server *serv, Client *client);
void	cmd_pass(std::vector<std::string> split, Server *serv, Client *client);
void	cmd_nick(std::vector<std::string> split, Server *serv, Client *client);
void	cmd_user(std::vector<std::string> split, Server *serv, Client *client);
void	cmd_quit(std::vector<std::string> split, Server *serv, Client *client);
void	cmd_part(std::vector<std::string> split, Server *serv, Client *client);



class Command
{
    private:
        	typedef void (*t_commandFunc)(std::vector<std::string> split, Server *serv, Client *client);
            t_commandFunc	_cmd;

        Command();
    public:
        Command(t_commandFunc cmd);
        ~Command();
        void    exe(std::vector<std::string> split, Server *serv, Client *client);
};

# include "Server.hpp"

#endif