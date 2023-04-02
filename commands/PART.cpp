/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:24:56 by acmaghou          #+#    #+#             */
/*   Updated: 2023/04/02 16:58:05 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

static void partChannel( irc::client& client_, std::string& channel_name_ ) {
	irc::channel* channel = irc::server::__serv->findChannel( channel_name_ );
	if ( channel == NULL ) client_._msg_out += ERR_NOSUCHCHANNEL( client_, channel_name_ );
	else {
		irc::channel::member_iterator it = channel->_members.find( &client_ );
		if ( it == channel->_members.end() )
			client_._msg_out += ERR_NOTONCHANNEL( client_, channel->_name );
		else {
			channel->broadcast(
				client_._message._params[ 1 ].empty() ?
					RPL_PART( client_, channel->_name ) :
					RPL_PARTMSG( client_, channel->_name, client_._message._params[ 1 ] ) );
			channel->_members.erase( it );
			client_.partChannel( channel->_name );
			if ( channel->_members.empty() ) irc::server::__serv->removeChannel( *channel );
		}
	}
}

void irc::commands::PART( irc::client& client_ ) {
	if ( client_._message._params.empty() || client_._message._params.front().empty() )
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
	else {
		std::vector< std::string > channel_names =
			irc::utils::split( client_._message._params[ 0 ], ',' );
		std::transform( channel_names.begin(), channel_names.end(), channel_names.begin(),
						irc::utils::toLower );
		for ( std::vector< std::string >::iterator it = channel_names.begin();
			  it != channel_names.end(); ++it )
			partChannel( client_, *it );
	}
}
