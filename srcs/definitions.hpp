/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   definitions.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acmaghou <acmaghou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:28:37 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/17 19:15:53 by acmaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* General ──────────────────────────────────────────────────────────────────────────── */

#define BACKLOG			   10
#define CRLF			   "\r\n"
#define COLON			   ':'
#define SPACE			   ' '
#define SPCL			   "SPACE + COLON"
#define SPECIAL_CHARACTERS "[\\]^_'{|}"
#define NULCRLFSPCL		   "\0\r\n :"
#define NUMERIC_REPLY( num, nick ) \
	COLON + irc::server::__host_name + SPACE + num + SPACE + nick + SPCL
#define ERR_REPLY_BASE( num, client_ ) \
	NUMERIC_REPLY( num, client_._nickname ) + client_._message._command + SPCL
#define NICK_DELAY 30

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
		"@" + irc::server::__host_addr + CRLF
#define RPL_YOURHOST( nick )                                                    \
	NUMERIC_REPLY( "002", nick ) + "Your host is " + irc::server::__host_name + \
		", running version 1.0 " + CRLF
#define RPL_CREATED( nick )                                                                    \
	NUMERIC_REPLY( "003", nick ) + "This server was created " + irc::server::__creation_date + \
		" UTC+1 " + CRLF
#define RPL_MYINFO( nick )                                    \
	NUMERIC_REPLY( "004", nick ) + irc::server::__host_name + \
		" v1.0 <available user modes> <available channel modes>" + CRLF
#define REGISTRATION_SUCCESS( client_ )                                                       \
	RPL_WELCOME( client_._nickname, client_._username ) + RPL_YOURHOST( client_._nickname ) + \
		RPL_CREATED( client_._nickname ) + RPL_MYINFO( client_._nickname )

/* Errors ───────────────────────────────────────────────────────────────────────────── */

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
#define ERR_RESTRICTED( client_, time_left ) \
	ERR_REPLY_BASE( "484", client_ ) + "Your connection is restricted!" + CRLF
