/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_server.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/14 18:26:15 by thverney          #+#    #+#             */
/*   Updated: 2021/04/14 18:34:08 by thverney         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

void	cmd_server(std::vector<std::string> split, Server *serv, Client *client)
{
    (void)split;
    std::vector<Client*> list = serv->getClients();
    std::vector<Client*>::iterator ite = list.begin();
	for (std::vector<Client*>::iterator it = list.end(); it != ite; ++ite)
    {
        if ((*it) == client)
        {
            (*it)->setIsServer(true);
            return;
        }
    }
}