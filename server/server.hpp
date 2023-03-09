/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 14:24:08 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/09 17:04:40 by sel-mars         ###   ########.fr       */
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
#include <ostream>		// ostream
#include <sstream>		// stringstream
#include <string>		// string
#include <sys/poll.h>	// pollfd - poll
#include <sys/socket.h> // get / free addrinfo - socket - bind - listen - accept - send - recv
#include <sys/types.h>	// ( BSD )
#include <unistd.h>		// close
#include <vector>		// vector

/* General ──────────────────────────────────────────────────────────────────────────── */

#define BACKLOG 10
#define CRLF	"\r\n"
#define COLON	':'
#define SPACE	' '

/* Exceptions ───────────────────────────────────────────────────────────────────────── */

#define THROW_EXCEPT( str )	   throw std::runtime_error( str )
#define ERRCODE_EXCEPT( code ) THROW_EXCEPT( std::strerror( code ) )
#define ERRNO_EXCEPT		   ERRCODE_EXCEPT( errno )
#define ARGS_EXCEPT			   THROW_EXCEPT( "Usage: ./ircserv <port> <password>" )
#define PORT_EXCEPT			   THROW_EXCEPT( "Port should be an integer in range [1, 65535]" )

/* Client Responses ─────────────────────────────────────────────────────────────────── */

#define NUMERIC_REPLY( num, target ) ":ircserv " num target
#define RPL_WELCOME( target )		 NUMERIC_REPLY( "001", target )
#define RPL_YOURHOST( target )		 NUMERIC_REPLY( "002", target )
#define RPL_CREATED( target )		 NUMERIC_REPLY( "003", target )
#define RPL_MYINFO( target )		 NUMERIC_REPLY( "004", target )
#define REGISTRATION_SUCCESS		 RPL_WELCOME CRLF RPL_YOURHOST CRLF RPL_CREATED CRLF RPL_MYINFO

/* Command Responses ────────────────────────────────────────────────────────────────── */

namespace irc {
	class message {
	  public:
		std::string				   _prefix;
		std::string				   _command;
		std::vector< std::string > _params;
		message( void ) {}
		~message( void ) {}
		void parseMsg( std::string& );
		void clear( void );
	};
	std::ostream& operator<<( std::ostream&, message& );
	class client {
	  public:
		user		_user;
		message		_message;
		std::string _msg_in;
		std::string _msg_out;
		client( void ) {}
		~client( void ) {}
	};
	class server {
	  private:
		char*				   _buff;
		unsigned short		   _port;
		std::string			   _password;
		std::string			   _welcome_msg;
		std::string			   _shutdown_msg;
		std::vector< pollfd >  _sockets;
		std::vector< client >  _clients;
		std::vector< user >	   _users;
		std::vector< channel > _channels;
		static server*		   __serv;
		void				   parse_args( const int&, char** );
		void				   sigHandler( int );
		static void			   staticSigHandler( int sg );
		void				   acceptClient( void );
		void disconClient( std::vector< pollfd >::iterator&, std::vector< client >::iterator& );
		void sendMsg( std::vector< pollfd >::iterator&, std::vector< client >::iterator& );
		void recvMsg( std::vector< pollfd >::iterator&, std::vector< client >::iterator& );

	  public:
		server( const int& ac, char** av );
		~server( void );
		void initServer( void );
		void runServer( void );
	};
} // namespace irc
