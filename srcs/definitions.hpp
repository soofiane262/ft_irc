/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   definitions.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:28:37 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/11 14:16:06 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* General ──────────────────────────────────────────────────────────────────────────── */

#define BACKLOG 10
#define CRLF	"\r\n"
#define COLON	':'
#define SPACE	' '
#define SPCLN	SPACE + COLON
#define NUMERIC_REPLY( num, nick ) \
	COLON + irc::server::__host_name + SPACE + num + SPACE + nick + SPCLN

/* Exceptions ───────────────────────────────────────────────────────────────────────── */

#define THROW_EXCEPT( str )	   throw std::runtime_error( str )
#define ERRCODE_EXCEPT( code ) THROW_EXCEPT( std::strerror( code ) )
#define ERRNO_EXCEPT		   ERRCODE_EXCEPT( errno )
#define ARGS_EXCEPT			   THROW_EXCEPT( "Usage: ./ircserv <port> <password>" )
#define PORT_EXCEPT			   THROW_EXCEPT( "<port> should be in range [1, 65535]" )
#define PASS_EXCEPT			   THROW_EXCEPT( "<password> shall not be empty" )

/* Client Responses ─────────────────────────────────────────────────────────────────── */

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

/* Command Responses ────────────────────────────────────────────────────────────────── */

#define ERR_UNKNOWNCOMMAND( client_ )                                               \
	NUMERIC_REPLY( "421", client_._nickname ) + client_._message._command + SPCLN + \
		"Unknown command" + CRLF

#define ERR_NEEDMOREPARAMS( client_ )                                               \
	NUMERIC_REPLY( "461", client_._nickname ) + client_._message._command + SPCLN + \
		"Not enough parameters" + CRLF

#define ERR_ALREADYREGISTRED( client_ )                                             \
	NUMERIC_REPLY( "462", client_._nickname ) + client_._message._command + SPCLN + \
		"Unauthorized command (already registered)" + CRLF

#define ERR_PASSWDMISMATCH( client_ )                                               \
	NUMERIC_REPLY( "464", client_._nickname ) + client_._message._command + SPCLN + \
		"Password incorrect" + CRLF
