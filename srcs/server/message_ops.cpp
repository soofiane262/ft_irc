/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_ops.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:04:07 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/20 17:15:24 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

/* send_message ───────────────────────────────────────────────────────────────────── */

void irc::server::sendMsg( client_iterator& client_it_ ) {
	if ( client_it_->second._msg_out.empty() ) return;
	int bytes_sent = send( client_it_->second._pfd.fd, client_it_->second._msg_out.c_str(),
						   client_it_->second._msg_out.length(), 0 );
	if ( bytes_sent == -1 ) THROW_EXCEPT( "Unable to send() data to client" );
	client_it_->second._msg_out.erase( 0, bytes_sent );
	if ( client_it_->second._pfd.revents & POLLHUP ) this->disconClient( client_it_ );
} // send_message

void irc::server::sendMsg( irc::client& client_ ) {
	if ( client_._msg_out.empty() ) return;
	int bytes_sent =
		send( client_._pfd.fd, client_._msg_out.c_str(), client_._msg_out.length(), 0 );
	if ( bytes_sent == -1 ) THROW_EXCEPT( "Unable to send() data to client" );
	client_._msg_out.erase( 0, bytes_sent );
} // send_message

/* receive_message ────────────────────────────────────────────────────────────────── */

void irc::server::recvMsg( irc::client& client_ ) {
	bzero( _buff, 513 );
	int bytes_rcvd = recv( client_._pfd.fd, _buff, 512, 0 );
	if ( bytes_rcvd == -1 ) {
		client_._msg_in.clear();
		THROW_EXCEPT( "Unable to recv() data from client" );
	}
	client_._msg_in.append( _buff, bytes_rcvd );
	if ( client_._msg_in.size() >= 2 &&
		 !client_._msg_in.compare( client_._msg_in.size() - 2, 2, CRLF ) )
		this->_commands[ client_ ];
} // receive_message
