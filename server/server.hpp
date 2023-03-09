/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 14:24:08 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/08 18:31:33 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../channel/channel.hpp" // channel
#include "../user/user.hpp"		  // user

#include <csignal>		// signal
#include <cstddef>		// size_t
#include <exception>	// exception
#include <fcntl.h>		// fcntl
#include <iostream>		// cout - cerr
#include <netdb.h>		// addrinfo
#include <sstream>		// stringstream
#include <string>		// string
#include <sys/poll.h>	// pollfd - poll
#include <sys/socket.h> // get / free addrinfo - socket - bind - listen - accept - send - recv
#include <sys/types.h>	// ( BSD )
#include <unistd.h>		// close
#include <vector>		// vector

#define THROW_EXCEPT( str )	   throw std::runtime_error( str )
#define ARGS_EXCEPT			   THROW_EXCEPT( "Usage: ./ircserv <port> <password>" )
#define ERRNO_EXCEPT		   THROW_EXCEPT( std::strerror( errno ) )
#define ERRCODE_EXCEPT( code ) THROW_EXCEPT( std::strerror( code ) )
#define PORT_EXCEPT			   THROW_EXCEPT( "Port should be an integer in range [1, 65535]" )
#define BACKLOG				   10

namespace irc {
	class client {
	  public:
		user		_user;
		std::string _msg_in;
		std::string _msg_out;
		client( void ) {}
		~client( void ) {}
	};
	class server {
	  private:
		char				  *_buff;
		unsigned short		   _port;
		std::string			   _password;
		std::string			   _welcome_msg;
		std::string			   _shutdown_msg;
		std::vector< pollfd >  _sockets;
		std::vector< client >  _clients;
		std::vector< user >	   _users;
		std::vector< channel > _channels;
		static server		  *__serv;
		void				   parse_args( const int &ac, char **av );
		void				   sigHandler( int sig );
		static void			   staticSigHandler( int sig );
		void				   acceptClient( void );
		void				   disconClient( const int client_pos );
		void				   sendMsg( const int client_pos );
		void				   recvMsg( const int client_pos );

	  public:
		server( const int &ac, char **av );
		~server( void );
		void initServer( void );
		void runServer( void );
	};
} // namespace irc
