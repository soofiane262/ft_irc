/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 14:24:08 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/10 18:42:59 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "definitions.hpp" // definitions

#include <arpa/inet.h>	// inet_ntoa
#include <csignal>		// signal
#include <cstddef>		// size_t
#include <ctime>		// time_t, struct tm, time, localtime
#include <exception>	// exception
#include <fcntl.h>		// fcntl
#include <iostream>		// cout, cerr
#include <map>			// map
#include <netdb.h>		// addrinfo, gethostbyname
#include <ostream>		// ostream
#include <sstream>		// stringstream
#include <string>		// string
#include <sys/poll.h>	// pollfd, poll
#include <sys/socket.h> // get / free addrinfo, socket, bind, listen, accept, send, recv
#include <sys/types.h>	// ( BSD )
#include <unistd.h>		// close
#include <vector>		// vector

namespace irc {
	/* message ─────────────────────────────────────────────────────────────────────────── */
	class message {
	  public:
		std::string				   _prefix, _command;
		std::vector< std::string > _params;
		message( void ) {}
		~message( void ) {}
		void parseMsg( std::string& );
		void clear( void );
	}; // message
	std::ostream& operator<<( std::ostream&, irc::message& );
	/* client ──────────────────────────────────────────────────────────────────────────── */
	class client {
	  public:
		int			 _fd;
		std::string	 _nickname, _username, _msg_in, _msg_out;
		irc::message _message;
		client( const int& fd ) : _fd( fd ), _nickname( "*" ), _username( "*" ) {}
		~client( void ) {}
	}; // client
	/* channel ─────────────────────────────────────────────────────────────────────────── */
	class channel {
	  public:
		std::string				   _name;
		std::vector< irc::client > _members;
		channel( void ) {}
		~channel( void ) {}
	}; // channel
	/* commands ────────────────────────────────────────────────────────────────────────── */
	class commands {
	  private:
		std::map< std::string,
				  std::string ( irc::commands::* )( std::vector< client >::iterator& ) >
					_commands;
		std::string pass( std::vector< irc::client >::iterator& );
		std::string nick( std::vector< irc::client >::iterator& );
		std::string user( std::vector< irc::client >::iterator& );

	  public:
		std::string operator[]( std::vector< irc::client >::iterator& );
		commands( void );
		~commands( void );
	}; // commands
	/* server ──────────────────────────────────────────────────────────────────────────── */
	class server {
	  private:
		char*						_buff;
		unsigned short				_port;
		std::string					_welcome_msg, _shutdown_msg;
		std::vector< pollfd >		_sockets;
		std::vector< irc::client >	_clients;
		std::vector< irc::channel > _channels;
		commands					_commands;
		void						parse_args( const int&, char** );
		static void					staticSigHandler( int sg );
		void						acceptClient( void );
		void						disconClient( std::vector< pollfd >::iterator&,
												  std::vector< irc::client >::iterator& );
		void sendMsg( std::vector< pollfd >::iterator&, std::vector< irc::client >::iterator& );
		void recvMsg( std::vector< pollfd >::iterator&, std::vector< irc::client >::iterator& );
		void handleMsg( std::vector< irc::client >::iterator& );
		void connectRegistr( std::vector< irc::client >::iterator& );

	  public:
		static server*	   __serv;
		static std::string __password, __host_name, __host_addr, __creation_date;
		server( const int& ac, char** av );
		~server( void );
		void initServer( void );
		void runServer( void );
		void shutDownServer( void );
	}; // server
} // namespace irc
