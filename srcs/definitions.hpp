/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   definitions.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:28:37 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/18 18:10:09 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* General ──────────────────────────────────────────────────────────────────────────── */

#define BACKLOG			   10
#define CRLF			   "\r\n"
#define COLON			   ':'
#define SPACE			   ' '
#define SPCL			   " :"
#define SPECIAL_CHARACTERS "[\\]^_'{|}"
#define NULCRLFSPCL		   "\0\r\n :"
#define NUMERIC_REPLY( num, nick ) \
	COLON + irc::server::__hostaddr + SPACE + num + SPACE + nick + SPCL
#define ERR_REPLY_BASE( num, client_ ) \
	NUMERIC_REPLY( num, client_._nickname ) + client_._message._command + SPCL
#define NICK_DELAY 30
enum user_modes {
	UMODE_AWAY		 = 1,
	UMODE_WALLOPS	 = 2,
	UMODE_INVISIBLE	 = 4,
	UMODE_RESTRICTED = 8,
	UMODE_OPERATOR	 = 16,
	UMODE_RECEIPT	 = 32
};

/* Exceptions ───────────────────────────────────────────────────────────────────────── */

#define THROW_EXCEPT( str )	   throw std::runtime_error( str )
#define ERRCODE_EXCEPT( code ) THROW_EXCEPT( std::strerror( code ) )
#define ERRNO_EXCEPT		   ERRCODE_EXCEPT( errno )
#define ARGS_EXCEPT			   THROW_EXCEPT( "Usage: ./ircserv <port> <password>" )
#define PORT_EXCEPT			   THROW_EXCEPT( "<port> should be in range [1, 65535]" )
#define PASS_EXCEPT			   THROW_EXCEPT( "<password> shall not be empty" )

/* Client Responses ─────────────────────────────────────────────────────────────────── */

/* Replies ──────────────────────────────────────────────────────────────────────────── */

#define RPL_WELCOME( nick, user )                                                                 \
	NUMERIC_REPLY( "001", nick ) + "Welcome to the Internet Relay Network " + nick + "!" + user + \
		"@" + irc::server::__hostaddr + CRLF
#define RPL_YOURHOST( nick )                                                   \
	NUMERIC_REPLY( "002", nick ) + "Your host is " + irc::server::__hostaddr + \
		", running version 1.0 " + CRLF
#define RPL_CREATED( nick )                                                                   \
	NUMERIC_REPLY( "003", nick ) + "This server was created " + irc::server::__creationdate + \
		" UTC+1 " + CRLF
#define RPL_MYINFO( nick )                                   \
	NUMERIC_REPLY( "004", nick ) + irc::server::__hostaddr + \
		" v1.0 <available user modes> <available channel modes>" + CRLF
#define REGISTRATION_SUCCESS( client_ )                                                       \
	RPL_WELCOME( client_._nickname, client_._username ) + RPL_YOURHOST( client_._nickname ) + \
		RPL_CREATED( client_._nickname ) + RPL_MYINFO( client_._nickname )
#define RPL_MOTDSTART( client_ )                                                                \
	NUMERIC_REPLY( "375", client_._nickname ) + "-" + SPACE + irc::server::__hostaddr + SPACE + \
		"Message of the day - " + CRLF
#define RPL_MOTD( client_, motd_ ) \
	NUMERIC_REPLY( "372", client_._nickname ) + "-" + SPACE + motd_ + CRLF
#define RPL_ENDOFMOTD( client_ ) \
	NUMERIC_REPLY( "376", client_._nickname ) + "End of MOTD command" + CRLF
#define RPL_LUSERCLIENT( client_ )                              \
	NUMERIC_REPLY( "251", client_._nickname ) + "There are " +  \
		irc::server::__serv->getClientsSize() + " users and " + \
		irc::server::__serv->getChannelsSize() + " channels on " + irc::server::__hostaddr + CRLF
#define RPL_LUSEROP( client_ ) \
	NUMERIC_REPLY( "252", client_._nickname ) + "0 :operator(s) online" + CRLF
#define RPL_LUSERUNKNOWN( client_ ) \
	NUMERIC_REPLY( "253", client_._nickname ) + "0 :unknown connection(s)" + CRLF
#define RPL_LUSERCHANNELS( client_ )                                                     \
	NUMERIC_REPLY( "254", client_._nickname ) + irc::server::__serv->getChannelsSize() + \
		" :channels formed" + CRLF
#define RPL_LUSERME( client_ )                              \
	NUMERIC_REPLY( "255", client_._nickname ) + "I have " + \
		irc::server::__serv->getClientsSize() + " clients and 0 servers" + CRLF

/* Errors ───────────────────────────────────────────────────────────────────────────── */

#define ERR_NOSUCHSERVER( client_, server_ ) \
	ERR_REPLY_BASE( "402", client_ ) + server_ + SPCL + "No such server" + CRLF
#define ERR_CLOSINGLINK( client_ )                                                  \
	ERR_REPLY_BASE( "404", client_ ) + "Closing Link: " + client_._nickname + "[" + \
		client_._username + "@" + irc::server::__hostaddr + "]" + CRLF
#define ERR_NOORIGIN( client_ )		  ERR_REPLY_BASE( "409", client_ ) + "No origin specified" + CRLF
#define ERR_NOCOMMANDGIVEN( client_ ) ERR_REPLY_BASE( "421", client_ ) + "No command given" + CRLF
#define ERR_UNKNOWNCOMMAND( client_ ) ERR_REPLY_BASE( "421", client_ ) + "Unknown command" + CRLF
#define ERR_NEEDMOREPARAMS( client_ ) \
	ERR_REPLY_BASE( "461", client_ ) + "Not enough parameters" + CRLF
#define ERR_ALREADYREGISTRED( client_ ) \
	ERR_REPLY_BASE( "462", client_ ) + "Unauthorized command (already registered)" + CRLF
#define ERR_PASSWDMISMATCH( client_ )  ERR_REPLY_BASE( "464", client_ ) + "Password incorrect" + CRLF
#define ERR_NONICKNAMEGIVEN( client_ ) ERR_REPLY_BASE( "431", client_ ) + "No nickname given" + CRLF
#define ERR_ERRONEUSNICKNAME( client_ ) \
	ERR_REPLY_BASE( "432", client_ ) + "Erroneous nickname" + CRLF
#define ERR_NICKNAMEINUSE( client_ ) \
	ERR_REPLY_BASE( "433", client_ ) + "Nickname is already in use" + CRLF
#define ERR_NICKCOLLISION( client_ ) ERR_REPLY_BASE( "436", client_ ) + "Nickname collision" + CRLF
#define ERR_UNAVAILRESOURCE( client_, time_left )                   \
	ERR_REPLY_BASE( "437", client_ ) + "Please wait " + time_left + \
		" seconds before attempting to change your nickname again." + CRLF
#define ERR_RESTRICTED( client_ ) \
	ERR_REPLY_BASE( "484", client_ ) + "Your connection is restricted!" + CRLF
