/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thverney <thverney@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 02:05:39 by aeoithd           #+#    #+#             */
/*   Updated: 2021/06/11 07:24:01 by thverney         ###   ########.fr       */
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
		bool	mods;
		std::string _flags;
		int limit;
		int currentUsers;
		std::list<Client*> banList;
		std::list<Client*> premiumUserList;

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
		void addFlag(char flags);
		void addUser();
		int getCurrentUsers();
		void userLeaveChannel();
		std::string getMods();
		void removeFlag(char flags);
		std::string getFlags();
		int getLimit();
		void setLimit(int max);
		std::list<Client*> getPremiumList();
		int checkPremiumList(std::string name);
		void addPremiumClient(Client *client);
		void removePremiumClient(Client *client);
		void addBanClient(Client *client);
		int checkBans(Client *client);

		std::string getTopic() const;
		std::string getChanName() const;
		std::list<Client*> getConnectedClients() const;
};

#include "Client.hpp"

#endif