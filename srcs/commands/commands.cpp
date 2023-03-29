/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:27:13 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/29 17:36:53 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

/* ctor ─────────────────────────────────────────────────────────────────────────────── */
irc::commands::commands( void ) {
	this->_commands[ "PASS" ]	 = &commands::PASS;
	this->_commands[ "NICK" ]	 = &commands::NICK;
	this->_commands[ "USER" ]	 = &commands::USER;
	this->_commands[ "PONG" ]	 = &commands::PONG;
	this->_commands[ "JOIN" ]	 = &commands::JOIN;
	this->_commands[ "QUIT" ]	 = &commands::QUIT;
	this->_commands[ "PRIVMSG" ] = &commands::PRIVMSG;
	this->_commands[ "PART" ]	 = &commands::PART;
	this->_commands[ "NOTICE" ]	 = &commands::NOTICE;
	this->_commands[ "MODE" ]	 = &commands::MODE;
	this->_commands[ "TOPIC" ]	 = &commands::TOPIC;
	this->_commands[ "LIST" ]	 = &commands::LIST;
	this->_commands[ "NAMES" ]	 = &commands::NAMES;
}

/* dtor ─────────────────────────────────────────────────────────────────────────────── */
irc::commands::~commands( void ) {}

/* operator[] ───────────────────────────────────────────────────────────────────────── */
void irc::commands::operator[]( irc::client& client_ ) {
	client_._message.parseMsg( client_._msg_in.erase( client_._msg_in.size() - 2 ) );
	if ( client_._message._command.empty() ) client_._msg_out += ERR_NOCOMMANDGIVEN( client_ );
	else {
		commands_iterator func_it = this->_commands.find( client_._message._command );
		if ( func_it == this->_commands.end() ) client_._msg_out += ERR_UNKNOWNCOMMAND( client_ );
		else
			( this->*func_it->second )( client_ );
	}
	client_._message.clear();
}
