/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:26:37 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/17 17:29:36 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/* ──────────────────────────────────────────────────────────────────────────────────── */
/* ──────────────────────────────────────────────────────────────────────────────────── */
// tmp - print message
std::ostream& irc::operator<<( std::ostream& o_, irc::message& message_ ) {
	o_ << "\033[2mprefix\t: ";
	o_ << "\n\033[2mcommand\t: ";
	if ( message_._command.empty() ) o_ << "<empty>";
	else
		o_ << "`\033[22m" << message_._command << "\033[2m`";
	o_ << "\n\033[2mparams\t: ";
	if ( message_._params.empty() ) o_ << "<empty>";
	else {
		o_ << "size " << message_._params.size();
		for ( std::size_t idx = 0; idx < message_._params.size(); ++idx )
			o_ << "\n" << idx << "\t--> `\033[22m" << message_._params[ idx ] << "\033[2m`";
	}
	o_ << "\033[22m\n";
	return o_;
}
// tmp
/* ──────────────────────────────────────────────────────────────────────────────────── */
/* ──────────────────────────────────────────────────────────────────────────────────── */

/* clear ────────────────────────────────────────────────────────────────────────────── */

void irc::message::clear( void ) {
	this->_command.clear();
	this->_params.clear();
} // clear

/* check_message ────────────────────────────────────────────────────────────────────── */

/* parse_message ────────────────────────────────────────────────────────────────────── */

void irc::message::parseMsg( std::string& msg_ ) {
	std::cout << msg_ << '\n';
	// tmp
	std::size_t pos;
	if ( !msg_.find( COLON ) ) {
		pos = msg_.find( SPACE );
		msg_.erase( 0, pos + ( pos != std::string::npos ) );
	}
	pos			   = msg_.find( SPACE );
	this->_command = msg_.substr( 0, pos );
	msg_.erase( 0, pos + ( pos != std::string::npos ) );
	for ( pos = msg_.find_first_not_of( SPACE ); !msg_.empty();
		  pos = msg_.find_first_not_of( SPACE ) ) {
		msg_.erase( 0, pos );
		if ( msg_.front() == COLON || this->_params.size() == 14 )
			pos = msg_.erase( 0, msg_.front() == COLON ).npos;
		else
			pos = msg_.find( SPACE );
		this->_params.push_back( msg_.substr( 0, pos ) );
		msg_.erase( 0, pos + ( pos != std::string::npos ) );
	}
	msg_.clear();
	// tmp
	std::cout << *this << '\n';
} // parse_message
