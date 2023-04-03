/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INVITE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 13:37:17 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/03 15:41:05 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::INVITE( irc::client& client_ ) {
	if ( client_._message._params.size() < 2 || client_._message._params[ 0 ].empty() ||
		 client_._message._params[ 1 ].empty() ) {
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
		return;
	}
	std::string	  channel_name = irc::utils::toLower( client_._message._params[ 1 ] );
	irc::client*  target	   = irc::server::__serv->findClient( client_._message._params[ 0 ] );
	irc::channel* channel	   = irc::server::__serv->findChannel( channel_name );
	if ( !target ) {
		client_._msg_out += ERR_NOSUCHNICK( client_, client_._message._params[ 0 ] );
	} else if ( channel ) {
		irc::channel::member_iterator member = channel->getMember( &client_ );
		if ( member == channel->_members.end() )
			client_._msg_out += ERR_NOTONCHANNEL( client_, channel->_name );
		else if ( channel->_mode & CMODE_INVITE && !( member->second & UMODE_CHANOP ) )
			client_._msg_out += ERR_CHANOPRIVSNEEDED( client_, channel->_name );
		else if ( target->isInChannel( channel->_name ) )
			client_._msg_out += ERR_USERONCHANNEL( client_, channel->_name );
		else {
			target->inviteChannel( channel->_name );
			target->_msg_out += RPL_INVITATION( client_, channel->_name, target->_nickname );
			client_._msg_out += RPL_INVITING( client_, channel->_name, target->_nickname );
			if ( target->_mode & UMODE_AWAY )
				client_._msg_out += RPL_AWAY( client_, target->_nickname, target->_away_msg );
		}
	}
}
