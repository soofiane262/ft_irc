/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:37:17 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/31 13:53:21 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::INVITE( irc::client& client_ ) {
	if ( client_._message._params.size() < 2 || client_._message._params[ 0 ].empty() ||
		 client_._message._params[ 1 ].empty() ) {
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
		return;
	}
	irc::client*  target  = irc::server::__serv->findClient( client_._message._params[ 0 ] );
	irc::channel* channel = irc::server::__serv->findChannel( client_._message._params[ 1 ] );
	if ( !target ) {
		client_._msg_out += ERR_NOSUCHNICK( client_, client_._message._params[ 0 ] );
	} else if ( channel ) {
		if ( target->isInChannel( channel->_name ) ) {
			client_._msg_out += ERR_USERONCHANNEL( client_, channel->_name );
		} else if ( channel->_mode & ( CMODE_INVITE | CMODE_PRIVATE | CMODE_SECRET ) ) {
			client_._msg_out += ERR_CHANOPRIVSNEEDED( client_, channel->_name );
		} else if ( target->_mode & UMODE_AWAY ) {
			target->_msg_out += RPL_AWAY( client_, target->_nickname, target->_away_msg );
		} else {
			target->inviteChannel( channel->_name );
			target->_msg_out += RPL_INVITING( client_, channel->_name, target->_nickname );
			client_._msg_out += RPL_INVITING( client_, channel->_name, target->_nickname );
		}
	}
}
