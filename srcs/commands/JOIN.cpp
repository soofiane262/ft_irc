/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:44:53 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/20 16:57:52 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::JOIN( irc::client& client_ ) {
	if ( client_._message._params.empty() ) client_._msg_out = ERR_NEEDMOREPARAMS( client_ );
	else if ( client_._message._params.front().at( 0 ) != '#' )
		client_._msg_out = ERR_NOSUCHCHANNEL( client_, client_._message._params.front() );
	else {
		irc::channel* channel =
			irc::server::__serv->findChannel( client_._message._params.front() );
		if ( channel == NULL ) {
			channel = new irc::channel( client_._message._params.front() );
			irc::server::__serv->addChannel( channel );
		}
		if ( channel->addMember( &client_ ) ) {
			for ( irc::channel::member_iterator it = channel->_members.begin();
				  it != channel->_members.end(); ++it )
				( *it )->_msg_out += RPL_JOIN( client_, channel->_name );
			if ( !channel->_topic.empty() )
				client_._msg_out += RPL_TOPIC( client_, channel->_name, channel->_topic );
			client_._msg_out += RPL_NAMREPLY( client_, channel->_name, channel->getMembers() );
			client_._msg_out += RPL_ENDOFNAMES( client_, channel->_name );
		} else {
			client_._msg_out = ERR_USERONCHANNEL( client_, channel->_name );
		}
	}
}
