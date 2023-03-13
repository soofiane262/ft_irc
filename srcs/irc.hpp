/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acmaghou <acmaghou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 14:24:08 by sel-mars          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2023/03/13 13:33:21 by sel-mars         ###   ########.fr       */
=======
/*   Updated: 2023/03/13 13:33:21 by sel-mars         ###   ########.fr       */
>>>>>>> origin/master
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
		pollfd&		 _pfd;
		std::string	 _nickname, _username, _msg_in, _msg_out;
		irc::message _message;
		client( pollfd& pfd ) : _pfd( pfd ), _nickname( "*" ), _username( "*" ) {}
		~client( void ) {}
	}; // client
	/* channel ─────────────────────────────────────────────────────────────────────────── */
	class channel {
	  public:
		typedef std::map< int, irc::client > clients_type;
		std::string							 _name;
		clients_type						 _members;
		channel( void ) {}
		~channel( void ) {}
	}; // channel
	/* commands ────────────────────────────────────────────────────────────────────────── */
	class commands {
	  private:
		typedef std::map< int, irc::client >::iterator clients_iterator;
		typedef std::map< std::string, std::string ( irc::commands::* )( irc::client& ) >
			commands_type;
		typedef std::map< std::string, std::string ( irc::commands::* )( irc::client& ) >::iterator
					  commands_iterator;
		commands_type _commands;
		std::string	  pass( irc::client& );
		std::string	  nick( irc::client& );
		std::string	  user( irc::client& );
		std::string	  quit( irc::client& );

	  public:
		std::string operator[]( irc::client& );
		commands( void );
		~commands( void );
	}; // commands
	/* server ──────────────────────────────────────────────────────────────────────────── */
	class server {
	  private:
		typedef std::map< int, irc::channel >			channels_type;
		typedef std::map< int, irc::channel >::iterator channels_iterator;
		typedef std::map< int, irc::client >			clients_type;
		typedef std::map< int, irc::client >::iterator	clients_iterator;
		char*											_buff;
		unsigned short									_port;
		std::string										_welcome_msg, _shutdown_msg;
		std::vector< pollfd >							_sockets;
		clients_type									_clients;
		channels_type									_channels;
		commands										_commands;
		void											parse_args( const int&, char** );
		static void										staticSigHandler( int sg );
		void											acceptClient( void );
		void											disconClient( clients_iterator& );
		void											sendMsg( irc::client& );
		void											recvMsg( irc::client& );
		void											handleMsg( irc::client& );
		void											connectRegistr( irc::client& );
		bool											checkNick( const std::string &nick );

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
