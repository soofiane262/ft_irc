/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:25:32 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/31 14:49:25 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::PRIVMSG( irc::client& client_ ) {
	if ( client_._message._params.empty() || client_._message._params.front().empty() )
		client_._msg_out += ERR_NORECIPIENT( client_ );
	else if ( client_._message._params.size() < 2 || client_._message._params[ 2 ].empty() )
		client_._msg_out += ERR_NOTEXTTOSEND( client_ );
	else if ( client_._message._params.front()[ 0 ] == '#' ) {
		irc::channel* channel =
			irc::server::__serv->findChannel( client_._message._params.front() );
		if ( channel == NULL )
			client_._msg_out += ERR_NOSUCHCHANNEL( client_, client_._message._params.front() );
		else {
			irc::channel::member_iterator member = channel->getMember( &client_ );
			if ( channel->_mode & CMODE_NOEXTERNAL && member == channel->_members.end() )
				client_._msg_out += ERR_NOTONCHANNEL( client_, channel->_name );
			else if ( channel->_mode & CMODE_MODERATED && member != channel->_members.end() &&
					  !( member->second & UMODE_VOICE ) )
				client_._msg_out += ERR_CANNOTSENDTOCHAN( client_, channel->_name );
			else {
				for ( irc::channel::member_iterator it = channel->_members.begin();
					  it != channel->_members.end(); ++it )
					if ( ( *it ).first->_nickname != client_._nickname )
						( *it ).first->_msg_out +=
							MSG( client_, channel->_name, client_._message._params.back() );
			}
		}
	} else {
		irc::client* user = irc::server::__serv->findClient( client_._message._params.front() );
		if ( user == NULL )
			client_._msg_out += ERR_NOSUCHNICK( client_, client_._message._params.front() );
		else
			user->_msg_out += MSG( client_, user->_nickname, client_._message._params.back() );
	}
}
