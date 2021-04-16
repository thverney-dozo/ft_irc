/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 02:21:10 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/28 15:44:04 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

Command::Command(t_commandFunc cmd) : _cmd(cmd) {}
Command::~Command() {}

void    Command::exe(std::vector<std::string> split, Server *serv, Client *client)
{
	this->_cmd(split, serv, client);
}

