/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 18:26:15 by thverney          #+#    #+#             */
/*   Updated: 2021/04/19 14:31:01 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmd_server(std::vector<std::string> split, Server *serv, Client *client)
{
    (void)split;
    std::map<int, Client*> list = serv->getClients();
    std::map<int, Client*>::iterator ite = list.begin();
	for (std::map<int, Client*>::iterator it = list.end(); it != ite; ++ite)
    {
        if ((*it).second == client)
        {
            (*it).second->setIsServer(true);
            return;
        }
    }
}