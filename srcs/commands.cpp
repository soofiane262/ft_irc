/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:27:13 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/17 17:35:08 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::stringstream ss;

irc::commands::commands( void ) {
	this->_commands[ "PASS" ] = &commands::pass;
	this->_commands[ "NICK" ] = &commands::nick;
	this->_commands[ "USER" ] = &commands::user;
	this->_commands[ "QUIT" ] = &commands::quit;
}

irc::commands::~commands( void ) {}

void irc::commands::operator[]( irc::client& client_ ) {
	client_._message.parseMsg( client_._msg_in.erase( client_._msg_in.size() - 2 ) );
	if ( client_._message._command.empty() ) client_._msg_out = ERR_NOCOMMANDGIVEN( client_ );
	commands_iterator func_it = this->_commands.find( client_._message._command );
	if ( func_it == this->_commands.end() ) client_._msg_out = ERR_UNKNOWNCOMMAND( client_ );
	client_._message.clear();
	( this->*func_it->second )( client_ );
}

void irc::commands::pass( irc::client& client_ ) {
	if ( client_._message._params.empty() ) client_._msg_out = ERR_NEEDMOREPARAMS( client_ );
	else if ( client_._realname.compare( "*" ) )
		client_._msg_out = ERR_ALREADYREGISTRED( client_ );
	else if ( client_._message._params.front().compare( irc::server::__password ) )
		client_._msg_out = ERR_PASSWDMISMATCH( client_ );
}

void irc::commands::nick( irc::client& client_ ) {
	std::time_t time_elapsed =
		std::difftime( std::time( NULL ), client_._nick_change + ( client_._nick_change == -1 ) );
	if ( client_._message._params.empty() || client_._message._params.front().empty() )
		client_._msg_out = ERR_NONICKNAMEGIVEN( client_ );
	else if ( !irc::utils::nickIsValid( client_._message._params.front() ) )
		client_._msg_out = ERR_ERRONEUSNICKNAME( client_ );
	else if ( irc::server::__serv->findClientByNick( client_._message._params.front() ) )
		client_._msg_out = client_._realname.compare( "*" ) ? ERR_NICKNAMEINUSE( client_ ) :
															  ERR_NICKCOLLISION( client_ );
	else if ( client_._nick_change != -1 && time_elapsed < NICK_DELAY ) {
		ss << ( NICK_DELAY - time_elapsed );
		client_._msg_out = ERR_UNAVAILRESOURCE( client_, ss.str() );
		ss.str( std::string( "" ) );
		ss.clear();
	} else {
		client_._nick_change = std::time( NULL );
		client_._nickname	 = client_._message._params.front();
	}
}

void irc::commands::user( irc::client& client_ ) {
	if ( client_._message._params.size() < 4 ) client_._msg_out = ERR_NEEDMOREPARAMS( client_ );
	else if ( client_._realname.compare( "*" ) )
		client_._msg_out = ERR_ALREADYREGISTRED( client_ );
	else {
		client_._username = client_._message._params[ 0 ];
		client_._realname = client_._message._params[ 3 ];
	}
}

void irc::commands::quit( irc::client& client_ ) {
	client_._msg_out = ERR_UNKNOWNCOMMAND( client_ );
}
