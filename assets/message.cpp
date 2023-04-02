/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:26:37 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/01 21:11:12 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::message::parseMsg( std::string& msg_ ) {
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
} // parse_message

void irc::message::clear( void ) {
	this->_command.clear();
	this->_params.clear();
} // clear_message

/* ──────────────────────────────────────────────────────────────────────────────────── */

void irc::server::sendMsg( irc::client& client_ ) {
	int bytes_sent = send( client_._fd, client_._msg_out.c_str(), client_._msg_out.length(), 0 );
	if ( bytes_sent == -1 ) THROW_EXCEPT( "Unable to send() data to client" );
	client_._msg_out.erase( 0, bytes_sent );
} // send_message

void irc::server::recvMsg( irc::client& client_ ) {
	bzero( _buff, 513 );
	int bytes_rcvd = recv( client_._fd, _buff, 512, 0 );
	if ( bytes_rcvd == -1 ) {
		client_._msg_in.clear();
		THROW_EXCEPT( "Unable to recv() data from client" );
	}
	client_._msg_in.append( _buff, bytes_rcvd );
	if ( client_._msg_in.size() >= 2 &&
		 !client_._msg_in.compare( client_._msg_in.size() - 2, 2, CRLF ) )
		this->_commands[ client_ ];
} // receive_message
