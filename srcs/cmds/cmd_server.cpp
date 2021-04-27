/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 18:26:15 by thverney          #+#    #+#             */
/*   Updated: 2021/04/27 13:21:27 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmd_server(std::vector<std::string> split, Server *serv, Client *client)
{
    serv->seg(4);
    (void)split;
    std::map<int, Client*> list = serv->getClients();
    std::map<int, Client*>::iterator ite = list.end();
	for (std::map<int, Client*>::iterator it = list.begin(); it != ite; it++)
    {
        if ((*it).second == client)
        {
            (*it).second->setIsServer(true);
            return;
        }
    }
}