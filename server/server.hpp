/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 14:24:08 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/09 19:01:38 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../channel/channel.hpp" // channel
#include "../user/user.hpp"		  // user

#include <arpa/inet.h>	// inet_ntoa
#include <csignal>		// signal
#include <cstddef>		// size_t
#include <ctime>		// time_t, struct tm, time, localtime
#include <exception>	// exception
#include <fcntl.h>		// fcntl
#include <iostream>		// cout, cerr
#include <netdb.h>		// addrinfo, gethostbyname
#include <ostream>		// ostream
#include <sstream>		// stringstream
#include <string>		// string
#include <sys/poll.h>	// pollfd, poll
#include <sys/socket.h> // get / free addrinfo, socket, bind, listen, accept, send, recv
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

#define NUMERIC_REPLY( host, num, nick ) ( COLON + host + SPACE + num + SPACE + nick )
#define RPL_WELCOME( host, nick, user )                                                        \
	( NUMERIC_REPLY( host, "001", nick ) + " :Welcome to the Internet Relay Network " + nick + \
	  "!" + user + "@" + host + CRLF )
#define RPL_YOURHOST( host, nick ) \
	( NUMERIC_REPLY( host, "002", nick ) + " :Your host is ircserv, running version 1.0 " + CRLF )
#define RPL_CREATED( host, nick, date )                                                        \
	( NUMERIC_REPLY( host, "003", nick ) + " :This server was created " + date + " UTC + 1 " + \
	  CRLF )
#define REGISTRATION_SUCCESS( host, nick, user, date )               \
	( RPL_WELCOME( host, nick, user ) + RPL_YOURHOST( host, nick ) + \
	  RPL_CREATED( host, nick, date ) )

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
		bool		_registered;
		user		_user;
		message		_message;
		std::string _msg_in;
		std::string _msg_out;
		client( void ) : _registered( false ) {}
		~client( void ) {}
	};
	class server {
	  private:
		char*		   _buff;
		std::string	   _host_addr;
		unsigned short _port;
		std::string	   _create_time;
		std::string	   _password;
		std::string	   _welcome_msg;
		std::string	   _shutdown_msg;
		// protoent*			   _host_infos;
		std::vector< pollfd >  _sockets;
		std::vector< client >  _clients;
		std::vector< user >	   _users;
		std::vector< channel > _channels;
		static server*		   __serv;
		void				   parse_args( const int&, char** );
		static void			   staticSigHandler( int sg );
		void				   sethostAddr( void );
		void				   acceptClient( void );
		void disconClient( std::vector< pollfd >::iterator&, std::vector< client >::iterator& );
		void sendMsg( std::vector< pollfd >::iterator&, std::vector< client >::iterator& );
		void recvMsg( std::vector< pollfd >::iterator&, std::vector< client >::iterator& );

	  public:
		server( const int& ac, char** av );
		~server( void );
		void initServer( void );
		void runServer( void );
		void shutDownServer( void );
	};
} // namespace irc
