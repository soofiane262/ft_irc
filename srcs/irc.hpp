/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 14:24:08 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/30 16:28:19 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "definitions.hpp" // definitions

#include <arpa/inet.h>	// inet_ntoa
#include <csignal>		// signal
#include <cstddef>		// size_t
#include <cstdlib>		// system
#include <cstring>		// strchr
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
#include <strings.h>	// bzero
#include <sys/poll.h>	// pollfd, poll
#include <sys/socket.h> // get / free addrinfo, socket, bind, listen, accept, send, recv
#include <sys/types.h>	// ( BSD )
#include <unistd.h>		// close
#include <vector>		// vector

namespace irc {
	/* utils ───────────────────────────────────────────────────────────────────────────── */
	class client;
	namespace utils {
		int						   ft_stoi( const std::string& str_ );
		bool					   nickIsValid( const std::string& );
		bool					   channelNameIsValid( const std::string& );
		unsigned char			   intToMode( const int& mode_ );
		bool					   pollfd_cmp( const pollfd&, const pollfd& );
		std::vector< std::string > split( const std::string&, const char& );
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
	class channel;
	class client {
	  public:
		typedef std::map< irc::client*, unsigned char >			  member_type;
		typedef std::map< irc::client*, unsigned char >::iterator member_iterator;
		int														  _fd;
		std::time_t												  _nick_change;
		unsigned char											  _mode;
		std::string												  last_channel;
		bool													  _quit;
		std::string _hostaddr, _hostname, _hostport, _nickname, _username, _realname, _msg_in,
			_msg_out;
		irc::message _message;
		bool		 has_mode( int );
		bool		 hasmodeOP(irc::channel& channel);
		client( int& fd )
			: _fd( fd ), _nick_change( -1 ), _mode( 0 ), _quit( false ), _nickname( "*" ),
			  _username( "*" ), _realname( "*" ) {}
		~client( void ) {}
	}; // client
	// tmp
	std::ostream& operator<<( std::ostream&, irc::client& );
	// tmp
	/* channel ─────────────────────────────────────────────────────────────────────────── */
	class channel {
	  public:
		typedef std::map< irc::client*, unsigned char >			  member_type;
		typedef std::map< irc::client*, unsigned char >::iterator member_iterator;

	  private:
		void addModes( const std::string& );
		void addModes( member_iterator, const std::string& );
		void delModes( const std::string& );
		void delModes( member_iterator, const std::string& );

	  public:
		std::string	   _key, _name, _topic;
		unsigned char  _mode;
		unsigned short _limit;
		member_type	   _members;
		channel( const std::string name_ = std::string() ) : _name( name_ ) {}
		~channel( void ) {}
		bool		addMember( irc::client* );
		std::string getMembers( void );
		std::string getModes( void );
		void		setModes( const std::string& );
		void		setModes( member_iterator, const std::string& );
		bool			isMember( irc::client* );
		member_iterator getMember( std::string& );
		member_iterator getMember( irc::client* );
		
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
		void		  PRIVMSG( irc::client& );
		void		  PART( irc::client& );
		void		  NOTICE( irc::client& );
		void		  MODE( irc::client& );
		void		  TOPIC( irc::client& );
		void		  LIST( irc::client& );
		void		  NAMES( irc::client& );
		void		  KICK( irc::client& );

	  public:
		void operator[]( irc::client& );

		commands( void );
		~commands( void );
	}; // commands
	/* server ──────────────────────────────────────────────────────────────────────────── */
	class server {
	  public:
		typedef std::vector< pollfd >							 poll_type;
		typedef std::vector< pollfd >::iterator					 poll_iterator;
		typedef std::map< std::string, irc::channel* >			 channel_type;
		typedef std::map< std::string, irc::channel* >::iterator channel_iterator;
		typedef std::map< int, irc::client >					 client_type;
		typedef std::map< int, irc::client >::iterator			 client_iterator;

	  private:
		char*		   _buff;
		unsigned short _port;
		poll_type	   _sockets;
		client_type	   _clients;
		channel_type   _channels;
		commands	   _commands;
		static void	   staticSigHandler( int sg );
		void		   acceptClient( void );
		void		   disconClient( client_iterator&, poll_iterator& );
		void		   recvMsg( irc::client& );
		void		   sendMsg( irc::client& );
		void		   connectRegistr( irc::client& );

	  public:
		static server*	   __serv;
		static std::string __password, __hostaddr, __creationdate;
		server( const int& ac, char** av );
		~server( void );
		void		  initServer( void );
		void		  runServer( void );
		void		  shutDownServer( void );
		std::string	  getClientsSize( void );
		std::string	  getChannelsSize( void );
		bool		  findClientByNick( const std::string& nick_ );
		irc::channel* addChannel( std::string& );
		irc::channel* findChannel( std::string& );
		irc::client*  findClient( const std::string& );
		void		  removeChannel( channel& );
		channel_type& getChannels( void );
	}; // server
} // namespace irc
