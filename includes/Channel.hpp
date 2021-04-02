/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaetan <gaetan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 02:05:39 by aeoithd           #+#    #+#             */
/*   Updated: 2021/04/02 08:39:11 by gaetan           ###   ########.fr       */
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
		std::string _topic;
		std::list<Client*> _clients;

		Channel();

	public:
		Channel(std::string const &name);
		Channel(std::string const &name, Client &client);
		Channel(const Channel &other);
		Channel &operator=(const Channel &other);
		virtual ~Channel();

		void addClient(Client *client);
		void removeClient(Client *client);
		void clientWriteMsg(std::string msg, Client *client);
		void userLeftChan(Client *client);
		void setTopic(std::string subject);

		std::string getTopic() const;
		std::string getChanName() const;
		std::list<Client*> getConnectedClients() const;
};

#include "Client.hpp"

#endif