/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 14:24:08 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/03 15:17:23 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "assets/definitions.hpp" // definitions

#include <arpa/inet.h>	// inet_ntoa
#include <csignal>		// signal
#include <cstddef>		// size_t
#include <cstdlib>		// system
#include <cstring>		// strchr
#include <ctime>		// time_t, struct tm, time, localtime
#include <curl/curl.h>	// curl
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

	namespace utils {
		std::string				   toLower( std::string& );
		int						   ft_stoi( const std::string& );
		unsigned char			   intToMode( const int& );
		bool					   nickIsValid( const std::string& );
		bool					   channelNameIsValid( const std::string& );
		bool					   pollfd_cmp( const pollfd&, const pollfd& );
		std::vector< std::string > split( const std::string&, const char& );
	} // namespace utils

	class message {
	  public:
		std::string				   _command;
		std::vector< std::string > _params;

		message( void ) {}
		~message( void ) {}

		void parseMsg( std::string& );
		void clear( void );
	}; // message

	class channel; // forward declaration
	class client {
	  public:
		typedef std::map< irc::client*, unsigned char >			  member_type;
		typedef std::map< irc::client*, unsigned char >::iterator member_iterator;
		typedef std::set< std::string >							  stringset_type;
		typedef std::set< std::string >::iterator				  stringset_iterator;
		typedef stringset_type									  channel_type;
		typedef stringset_iterator								  channel_iterator;

		int			   _fd;
		std::time_t	   _sign, _idle, _nick_change;
		unsigned char  _mode;
		bool		   _quit;
		std::string	   _hostaddr, _nickname, _username, _realname, _msg_in, _msg_out, _away_msg;
		irc::message   _message;
		channel_type   _channels_joined;
		channel_type   _channels_invited;
		stringset_type _clients_connnected;

		client( int& fd )
			: _fd( fd ), _nick_change( -1 ), _mode( 0 ), _quit( false ), _nickname( "*" ),
			  _username( "*" ), _realname( "*" ) {}
		~client( void ) {}

		std::string who( std::string& );
		std::string who( irc::channel& );
		std::string getModes( void );
		std::string getChannels( char );
		std::string getSignOnTime( void );
		std::string getIdleTime( void );
		bool		isInvited( std::string& );
		bool		isInChannel( std::string& );
		void		inviteChannel( std::string& );
		void		joinChannel( std::string& );
		void		partChannel( std::string& );
	}; // client

	class channel {
	  public:
		typedef std::map< irc::client*, unsigned char >			  member_type;
		typedef std::map< irc::client*, unsigned char >::iterator member_iterator;

	  public:
		std::string	   _key, _name, _topic;
		unsigned char  _mode;
		unsigned short _limit;
		member_type	   _members;

		channel( const std::string name_ = std::string() ) : _name( name_ ) {}
		~channel( void ) {}

		void			broadcast( std::string );
		void			broadcast( irc::client&, std::string );
		bool			isMember( irc::client* );
		bool			addMember( irc::client*, std::string& );
		std::string		getMembers( void );
		std::string		getModes( void );
		void			setModes( const std::string& );
		void			setModes( member_iterator, const std::string& );
		member_iterator getMember( std::string& );
		member_iterator getMember( irc::client* );
		member_iterator getMemberByUsername( std::string& );
	}; // channel

	class commands {
	  private:
		typedef std::map< int, irc::client >::iterator							   client_iterator;
		typedef std::map< std::string, void ( irc::commands::* )( irc::client& ) > commands_type;
		typedef std::map< std::string, void ( irc::commands::* )( irc::client& ) >::iterator
					  commands_iterator;
		commands_type _commands;
		void		  kick_member( irc::client&, std::string&, irc::channel*, std::string& );
		void		  AWAY( irc::client& );
		void		  BOT( irc::client& );
		void		  DIE( irc::client& );
		void		  INVITE( irc::client& );
		void		  JOIN( irc::client& );
		void		  KICK( irc::client& );
		void		  LIST( irc::client& );
		void		  MODE( irc::client& );
		void		  NAMES( irc::client& );
		void		  NICK( irc::client& );
		void		  NOTICE( irc::client& );
		void		  OPER( irc::client& );
		void		  PART( irc::client& );
		void		  PASS( irc::client& );
		void		  PONG( irc::client& );
		void		  PRIVMSG( irc::client& );
		void		  QUIT( irc::client& );
		void		  RESTART( irc::client& );
		void		  TOPIC( irc::client& );
		void		  USER( irc::client& );
		void		  WALLOPS( irc::client& );
		void		  WHO( irc::client& );
		void		  WHOIS( irc::client& );

	  public:
		commands( void );
		~commands( void ) {}
		void operator[]( irc::client& );
	}; // commands

	class server {
	  public:
		typedef std::vector< pollfd >							poll_type;
		typedef std::vector< pollfd >::iterator					poll_iterator;
		typedef std::map< std::string, irc::channel >			channel_type;
		typedef std::map< std::string, irc::channel >::iterator channel_iterator;
		typedef std::map< int, irc::client >					client_type;
		typedef std::map< int, irc::client >::iterator			client_iterator;

	  private:
		char*		   _buff;
		unsigned short _port;
		poll_type	   _sockets;
		client_type	   _clients;
		channel_type   _channels;
		commands	   _commands;

		static void staticSigHandler( int sg );
		void		acceptClient( void );
		void		disconClient( client_iterator&, poll_iterator& );
		void		recvMsg( irc::client& );
		void		sendMsg( irc::client& );
		void		connectRegistr( irc::client& );

	  public:
		static server*	   __serv;
		static bool		   __shutdown, __restart;
		static std::string __password, __operpass, __hostaddr, __creationdate;
		server( const int& ac, char** av );
		~server( void );
		void execCommand( irc::client& );
		/* Server initialisation, run and shutdown ────────────────────────────────────────── */
		void initServer( void );
		void runServer( void );
		void shutdownServer( void );
		/* Server statistics ──────────────────────────────────────────────────────────────── */
		std::string getClientsSize( void );
		std::string getOpersSize( void );
		std::string getChannelsSize( void );
		/* Server client and channel management ──────────────────────────────────────────── */
		bool		  isClient( const std::string& );
		irc::client*  findClient( const std::string& );
		client_type&  getClients( void );
		irc::channel* addChannel( std::string& );
		irc::channel* findChannel( std::string& );
		channel_type& getChannels( void );
		void		  removeChannel( channel& );
		/* Broadcasting ───────────────────────────────────────────────────────────────────── */
		void broadcastWallops( irc::client& );
		void broadcastJoinedChannels( irc::client&, std::string );
	}; // server

} // namespace irc
