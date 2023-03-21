/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acmaghou <acmaghou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 14:24:08 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/21 08:36:14 by acmaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "definitions.hpp" // definitions

#include <arpa/inet.h>	// inet_ntoa
#include <csignal>		// signal
#include <cstddef>		// size_t
#include <cstdlib>		// system
#include <ctime>		// time_t, struct tm, time, localtime
#include <exception>	// exception
#include <fcntl.h>		// fcntl
#include <fstream>		// ifstream
#include <iostream>		// cout, cerr
#include <map>			// map
#include <netdb.h>		// addrinfo, gethostbyname
#include <ostream>		// ostream
#include <set>			// set
#include <sstream>		// stringstream
#include <string>		// string
#include <sys/poll.h>	// pollfd, poll
#include <sys/socket.h> // get / free addrinfo, socket, bind, listen, accept, send, recv
#include <sys/types.h>	// ( BSD )
#include <unistd.h>		// close
#include <vector>		// vector
#include <strings.h>	// bzero
#include <cstring>		// strchr


namespace irc {
	/* utils ───────────────────────────────────────────────────────────────────────────── */
	namespace utils {
		bool		  nickIsValid( const std::string& nick_ );
		unsigned char intToMode( const int& mode_ );
	} // namespace utils
	/* message ─────────────────────────────────────────────────────────────────────────── */
	class message {
	  public:
		std::string				   _command;
		std::vector< std::string > _params;
		message( void ) {}
		~message( void ) {}
		void parseMsg( std::string& );
		void checkMsg( std::string& );
		void clear( void );
	}; // message
	// tmp
	std::ostream& operator<<( std::ostream&, irc::message& );
	// tmp
	/* client ──────────────────────────────────────────────────────────────────────────── */
	class	channel;
	class client {
	  public:
	  	typedef std::map< std::string, irc::channel >					channels_type;
		typedef std::map< std::string, irc::channel >::iterator			channels_iterator;

		channels_type	_channels;
		pollfd&			_pfd;
		std::time_t		_nick_change;
		unsigned char	_mode;
		std::string		_hostaddr, _hostname, _hostport, _nickname, _username, _realname, _msg_in,
			_msg_out;
		irc::message _message;
		client( pollfd& pfd )
			: _pfd( pfd ), _nick_change( -1 ), _mode( 0 ), _nickname( "*" ), _username( "*" ),
			  _realname( "*" ) {}
		~client( void ) {}
	}; // client
	// tmp
	std::ostream& operator<<( std::ostream&, irc::client& );
	// tmp
	/* channel ─────────────────────────────────────────────────────────────────────────── */
	class channel {
	  public:
		typedef std::set< irc::client* >		   member_type;
		typedef std::set< irc::client* >::iterator member_iterator;
		std::string								   _name, _topic;
		unsigned char							   _mode;
		member_type								   _members;
		channel( const std::string name_ = std::string() ) : _name( name_ ) {}
		~channel( void ) {}
		bool		addMember( irc::client* );
		std::string getMembers( void );
	}; // channel
	/* commands ────────────────────────────────────────────────────────────────────────── */
	class commands {
	  private:
		typedef std::map< int, irc::client >::iterator							   client_iterator;
		typedef std::map< std::string, void ( irc::commands::* )( irc::client& ) > commands_type;
		typedef std::map< std::string, void ( irc::commands::* )( irc::client& ) >::iterator
					  commands_iterator;
		commands_type _commands;
		void		  PASS( irc::client& );
		void		  NICK( irc::client& );
		void		  USER( irc::client& );
		void		  PONG( irc::client& );
		void		  JOIN( irc::client& );
		void		  QUIT( irc::client& );
		void		  PRIVMSG ( irc::client& );

	  public:
		void operator[]( irc::client& );
		
		commands( void );
		~commands( void );
	}; // commands
	/* server ──────────────────────────────────────────────────────────────────────────── */
	class server {
	  public:
		typedef std::map< std::string, irc::channel* >			 channel_type;
		typedef std::map< std::string, irc::channel* >::iterator channel_iterator;
		typedef std::map< int, irc::client >					 client_type;
		typedef std::map< int, irc::client >::iterator			 client_iterator;

	  private:
		char*				  _buff;
		unsigned short		  _port;
		std::vector< pollfd > _sockets;
		client_type			  _clients;
		channel_type		  _channels;
		commands			  _commands;
		void				  parse_args( const int&, char** );
		static void			  staticSigHandler( int sg );
		void				  acceptClient( void );
		void				  disconClient( client_iterator& );
		void				  recvMsg( irc::client& );
		void				  sendMsg( irc::client& );
		void				  sendMsg( client_iterator& client_it_ );
		void				  connectRegistr( irc::client& );

	  public:
		static server*	   __serv;
		static std::string __password, __hostaddr, __creationdate;
		server( const int& ac, char** av );
		~server( void );
		bool		  findClientByNick( const std::string& nick_ );
		std::string	  getClientsSize( void );
		std::string	  getChannelsSize( void );
		void		  initServer( void );
		void		  runServer( void );
		void		  addChannel( irc::channel* );
		void		  shutDownServer( void );
		irc::channel* findChannel( std::string& );
	}; // server
} // namespace irc
