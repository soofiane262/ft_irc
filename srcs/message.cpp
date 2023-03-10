/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:26:37 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/10 18:41:44 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/* ──────────────────────────────────────────────────────────────────────────────────── */
/* ──────────────────────────────────────────────────────────────────────────────────── */
// tmp - print message
std::ostream& irc::operator<<( std::ostream& o_, irc::message& message_ ) {
	o_ << "\033[2mprefix\t: ";
	if ( message_._prefix.empty() ) o_ << "<empty>";
	else
		o_ << "`\033[22m" << message_._prefix << "\033[2m`";
	o_ << "\n\033[2mcommand\t: ";
	if ( message_._command.empty() ) o_ << "<empty>";
	else
		o_ << "`\033[22m" << message_._command << "\033[2m`";
	o_ << "\n\033[2mparams\t: ";
	if ( message_._params.empty() ) o_ << "<empty>";
	else
		for ( std::size_t idx = 0; idx < message_._params.size(); ++idx )
			o_ << "\n" << idx << "\t--> `\033[22m" << message_._params[ idx ] << "\033[2m`";
	o_ << "\033[22m\n";
	return o_;
}
// tmp
/* ──────────────────────────────────────────────────────────────────────────────────── */
/* ──────────────────────────────────────────────────────────────────────────────────── */

/* clear ────────────────────────────────────────────────────────────────────────────── */

void irc::message::clear( void ) {
	this->_prefix.clear();
	this->_command.clear();
	this->_params.clear();
} // clear

/* parse_message ────────────────────────────────────────────────────────────────────── */

void irc::message::parseMsg( std::string& msg_ ) {
	if ( !msg_.find( COLON ) ) {
		this->_prefix = msg_.erase( 0, 1 ).substr( 0, msg_.find( SPACE ) );
		msg_.erase( 0, msg_.find( SPACE ) + 1 );
	}
	this->_command = msg_.substr( 0, msg_.find( SPACE ) );
	msg_.erase( 0, msg_.find( SPACE ) + 1 );
	for ( std::size_t pos = msg_.find_first_not_of( SPACE ); !msg_.empty();
		  pos			  = msg_.find_first_not_of( SPACE ) ) {
		pos == std::string::npos ? msg_.erase( 0 ) : msg_.erase( 0, pos );
		pos = msg_.front() == COLON ? msg_.erase( 0, 1 ).npos : msg_.find( SPACE );
		this->_params.push_back( msg_.substr( 0, pos ) );
		pos == std::string::npos ? msg_.erase( 0 ) : msg_.erase( 0, pos + 1 );
	}
	msg_.clear();
} // parse_message
