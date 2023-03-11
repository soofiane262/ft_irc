/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:27:13 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/11 12:50:04 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

#include <utility>

irc::commands::commands( void ) {
	this->_commands[ "PASS" ] = &commands::pass;
	this->_commands[ "NICK" ] = &commands::nick;
	this->_commands[ "USER" ] = &commands::user;
}

irc::commands::~commands( void ) {}

std::string irc::commands::operator[]( std::vector< client >::iterator& client_it_ ) {
	std::map< std::string, std::string ( irc::commands::* )(
							   std::vector< client >::iterator& ) >::iterator func_it =
		this->_commands.find( client_it_->_message._command );
	if ( func_it == this->_commands.end() ) return ERR_UNKNOWNCOMMAND( client_it_ );
	return ( this->*func_it->second )( client_it_ );
}

std::string irc::commands::pass( std::vector< client >::iterator& client_it_ ) {
	std::string result;
	if ( client_it_->_message._params.empty() ) result = ERR_NEEDMOREPARAMS( client_it_ );
	else if ( client_it_->_nickname.compare( "*" ) )
		result = ERR_ALREADYREGISTRED( client_it_ );
	else if ( client_it_->_message._params.front().compare( irc::server::__password ) )
		result = ERR_PASSWDMISMATCH( client_it_ );
	return result;
}

std::string irc::commands::nick( std::vector< client >::iterator& client_it_ ) {
	(void)client_it_;
	std::string result;
	return result;
}

std::string irc::commands::user( std::vector< client >::iterator& client_it_ ) {
	(void)client_it_;
	std::string result;
	return result;
}
