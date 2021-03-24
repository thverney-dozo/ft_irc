/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeoithd <aeoithd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/23 19:21:49 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/24 02:26:28 by aeoithd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

# include "Client.hpp"
# include <vector>


class Server;

void	cmd_join(std::vector<std::string> split, Server *serv, Client *client);

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