/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 14:24:08 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/10 13:17:08 by sel-mars         ###   ########.fr       */
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

#define NUMERIC_REPLY( num, nick ) COLON + irc::server::__host_name + SPACE + num + SPACE + nick
#define RPL_WELCOME( nick, user )                                                            \
	NUMERIC_REPLY( "001", nick ) + " :Welcome to the Internet Relay Network " + nick + "!" + \
		user + "@" + irc::server::__host_addr + CRLF
#define RPL_YOURHOST( nick )                                                      \
	NUMERIC_REPLY( "002", nick ) + " :Your host is " + irc::server::__host_name + \
		", running version 1.0 " + CRLF
#define RPL_CREATED( nick )                                                                      \
	NUMERIC_REPLY( "003", nick ) + " :This server was created " + irc::server::__creation_date + \
		" UTC+1 " + CRLF
#define RPL_MYINFO( nick )                                           \
	NUMERIC_REPLY( "004", nick ) + " :" + irc::server::__host_name + \
		" v1.0 <available user modes> <available channel modes>" + CRLF
#define REGISTRATION_SUCCESS( nick, user ) \
	RPL_WELCOME( nick, user ) + RPL_YOURHOST( nick ) + RPL_CREATED( nick ) + RPL_MYINFO( nick )

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
		static server*		   __serv;
		static std::string	   __host_name;
		static std::string	   __host_addr;
		static std::string	   __creation_date;
		char*				   _buff;
		unsigned short		   _port;
		std::string			   _password;
		std::string			   _welcome_msg;
		std::string			   _shutdown_msg;
		std::vector< pollfd >  _sockets;
		std::vector< client >  _clients;
		std::vector< user >	   _users;
		std::vector< channel > _channels;
		void				   parse_args( const int&, char** );
		static void			   staticSigHandler( int sg );
		void				   sethostAddr( void );
		void				   acceptClient( void );
		void disconClient( std::vector< pollfd >::iterator&, std::vector< client >::iterator& );
		void sendMsg( std::vector< pollfd >::iterator&, std::vector< client >::iterator& );
		void recvMsg( std::vector< pollfd >::iterator&, std::vector< client >::iterator& );
		void handleMsg( std::vector< client >::iterator& client_it_ );

	  public:
		server( const int& ac, char** av );
		~server( void );
		void initServer( void );
		void runServer( void );
		void shutDownServer( void );
	};
} // namespace irc
