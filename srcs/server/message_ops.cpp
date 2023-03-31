/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message_ops.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:04:07 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/31 14:15:50 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

/* receive_message ────────────────────────────────────────────────────────────────── */

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

/* send_message ───────────────────────────────────────────────────────────────────── */

void irc::server::sendMsg( irc::client& client_ ) {
	int bytes_sent = send( client_._fd, client_._msg_out.c_str(), client_._msg_out.length(), 0 );
	if ( bytes_sent == -1 ) THROW_EXCEPT( "Unable to send() data to client" );
	client_._msg_out.erase( 0, bytes_sent );
} // send_message
