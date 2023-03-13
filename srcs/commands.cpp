/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:27:13 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/13 13:36:10 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

#include <utility>

irc::commands::commands( void ) {
	this->_commands[ "PASS" ] = &commands::pass;
	this->_commands[ "NICK" ] = &commands::nick;
	this->_commands[ "USER" ] = &commands::user;
	this->_commands[ "QUIT" ] = &commands::quit;
}

irc::commands::~commands( void ) {}

std::string irc::commands::operator[]( irc::client& client_ ) {
	commands_iterator func_it = this->_commands.find( client_._message._command );
	if ( func_it == this->_commands.end() ) return ERR_UNKNOWNCOMMAND( client_ );
	return ( this->*func_it->second )( client_ );
}

std::string irc::commands::pass( irc::client& client_ ) {
	std::string result;
	if ( client_._message._params.empty() ) result = ERR_NEEDMOREPARAMS( client_ );
	else if ( client_._nickname.compare( "*" ) )
		result = ERR_ALREADYREGISTRED( client_ );
	else if ( client_._message._params.front().compare( irc::server::__password ) )
		result = ERR_PASSWDMISMATCH( client_ );
	return result;
}

std::string irc::commands::nick( irc::client& client_ ) {
	(void)client_;
	std::string result;
	return result;
}

std::string irc::commands::user( irc::client& client_ ) {
	(void)client_;
	std::string result;
	return result;
}

std::string irc::commands::quit( irc::client& client_ ) { return ERR_UNKNOWNCOMMAND( client_ ); }
