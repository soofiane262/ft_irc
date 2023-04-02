/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:27:13 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/02 12:54:46 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

irc::commands::commands( void ) {
	this->_commands[ "AWAY" ]	 = &commands::AWAY;
	this->_commands[ "BOT" ]	 = &commands::BOT;
	this->_commands[ "DIE" ]	 = &commands::DIE;
	this->_commands[ "INVITE" ]	 = &commands::INVITE;
	this->_commands[ "JOIN" ]	 = &commands::JOIN;
	this->_commands[ "KICK" ]	 = &commands::KICK;
	this->_commands[ "LIST" ]	 = &commands::LIST;
	this->_commands[ "MODE" ]	 = &commands::MODE;
	this->_commands[ "NAMES" ]	 = &commands::NAMES;
	this->_commands[ "NICK" ]	 = &commands::NICK;
	this->_commands[ "NOTICE" ]	 = &commands::NOTICE;
	this->_commands[ "OPER" ]	 = &commands::OPER;
	this->_commands[ "PART" ]	 = &commands::PART;
	this->_commands[ "PASS" ]	 = &commands::PASS;
	this->_commands[ "PONG" ]	 = &commands::PONG;
	this->_commands[ "PRIVMSG" ] = &commands::PRIVMSG;
	this->_commands[ "QUIT" ]	 = &commands::QUIT;
	this->_commands[ "RESTART" ] = &commands::RESTART;
	this->_commands[ "TOPIC" ]	 = &commands::TOPIC;
	this->_commands[ "USER" ]	 = &commands::USER;
	this->_commands[ "WHO" ]	 = &commands::WHO;
	this->_commands[ "WHOIS" ]	 = &commands::WHOIS;
} // ctor

void irc::commands::operator[]( irc::client& client_ ) {
	client_._message.clear();
	client_._message.parseMsg( client_._msg_in.erase( client_._msg_in.size() - 2 ) );
	if ( client_._message._command.empty() ) client_._msg_out += ERR_NOCOMMANDGIVEN( client_ );
	else {
		commands_iterator func_it = this->_commands.find( client_._message._command );
		if ( func_it == this->_commands.end() ) client_._msg_out += ERR_UNKNOWNCOMMAND( client_ );
		else
			( this->*func_it->second )( client_ );
	}
	client_._message.clear();
} // operator[]
