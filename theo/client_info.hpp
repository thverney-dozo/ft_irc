/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_info.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aeoithd <aeoithd@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/15 16:24:42 by aeoithd           #+#    #+#             */
/*   Updated: 2021/03/15 16:37:40 by aeoithd          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_INFO
# define CLIENT_INFO

#include <arpa/inet.h>

typedef struct   s_client_info
{
    struct sockaddr_in inf; // all client info (ip, port, os)
    int                 fd; // file descriptor of client
    void                *next;
}               t_client_info;

#endif