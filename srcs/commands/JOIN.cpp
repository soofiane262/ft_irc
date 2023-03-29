/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JOIN.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:44:53 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/29 18:11:43 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

static void joinChannel( irc::client& client_, std::string& channel_name_,
						 std::string& channel_key_ ) {
	if ( !irc::utils::channelNameIsValid( channel_name_ ) )
		client_._msg_out += ERR_NOSUCHCHANNEL( client_, channel_name_ );
	else {
		irc::channel* channel = irc::server::__serv->findChannel( channel_name_ );
		if ( channel == NULL ) channel = irc::server::__serv->addChannel( channel_name_ );
		if ( channel->_mode & CMODE_LIMIT &&
			 static_cast< unsigned short >( channel->_members.size() ) == channel->_limit )
			client_._msg_out += ERR_CHANNELISFULL( client_, channel->_name );
		else if ( channel->_mode & CMODE_KEY && channel->_key != channel_key_ )
			client_._msg_out += ERR_BADCHANNELKEY( client_, channel->_name );
		else {
			if ( channel->addMember( &client_ ) ) {
				for ( irc::channel::member_iterator it = channel->_members.begin();
					  it != channel->_members.end(); ++it )
					( *it ).first->_msg_out += RPL_JOIN( client_, channel->_name );
				if ( !channel->_topic.empty() )
					client_._msg_out += RPL_TOPIC( client_, channel->_name, channel->_topic );
				client_._msg_out += RPL_NAMREPLY( client_, channel->_name, channel->getMembers() );
				client_._msg_out += RPL_ENDOFNAMES( client_, channel->_name );
			} else
				client_._msg_out += ERR_USERONCHANNEL( client_, channel->_name );
		}
	}
}

void irc::commands::JOIN( irc::client& client_ ) {
	if ( client_._message._params.empty() || client_._message._params.front().empty() )
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
	else {
		std::vector< std::string > channel_names =
			irc::utils::split( client_._message._params[ 0 ], ',' );
		std::vector< std::string > channel_keys =
			client_._message._params.size() > 1 ?
				irc::utils::split( client_._message._params[ 1 ], ',' ) :
				std::vector< std::string >();
		std::string channel_key;
		for ( std::vector< std::string >::iterator it = channel_names.begin();
			  it != channel_names.end(); ++it, channel_key.clear() ) {
			if ( !channel_keys.empty() ) {
				channel_key = channel_keys.front();
				channel_keys.erase( channel_keys.begin() );
			}
			joinChannel( client_, *it, channel_key );
		}
	}
}
