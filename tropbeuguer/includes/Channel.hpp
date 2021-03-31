/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeoithd <aeoithd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 02:05:39 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/24 02:05:47 by aeoithd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <iostream>
#include <list>
#include <unistd.h>

class Client;

class Channel
{
	private:
		std::string _name;
		std::list<Client*> _clients;

		Channel();

	public:
		Channel(std::string const &name);
		Channel(std::string const &name, Client &client);
		Channel(const Channel &other);
		Channel &operator=(const Channel &other);
		virtual ~Channel();

		void addClient(Client *client);
		void clientWriteMsg(std::string msg, Client *client);

		std::string getChanName() const;
		std::list<Client*> getConnectedClients() const;
};

#include "Client.hpp"

#endif