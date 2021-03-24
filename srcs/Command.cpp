/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeoithd <aeoithd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 02:21:10 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/24 02:21:29 by aeoithd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmd_join(std::vector<std::string> split, Server *serv, Client *client);


Command::Command(t_commandFunc cmd) : _cmd(cmd) {}
Command::~Command() {}

void    Command::exe(std::vector<std::string> split, Server *serv, Client *client)
{
	this->_cmd(split, serv, client);
}

