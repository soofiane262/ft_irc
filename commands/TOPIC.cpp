/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 14:26:21 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/31 21:32:35 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::TOPIC( irc::client& client_ ) {
	if ( client_._message._params.empty() || client_._message._params.front().empty() ) {
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
	} else {
		std::string	  channel_name = irc::utils::toLower( client_._message._params[ 0 ] );
		irc::channel* channel	   = irc::server::__serv->findChannel( channel_name );
		if ( channel == NULL ) {
			client_._msg_out += ERR_NOSUCHCHANNEL( client_, channel_name );
		} else if ( client_._message._params.size() == 1 && !channel->_topic.empty() ) {
			client_._msg_out += RPL_TOPIC( client_, channel->_name, channel->_topic );
		} else if ( ( client_._message._params.size() == 1 && channel->_topic.empty() ) ) {
			client_._msg_out += RPL_NOTOPIC( client_, channel->_name );
		} else {
			irc::channel::member_iterator member = channel->getMember( &client_ );
			if ( member == channel->_members.end() )
				client_._msg_out += ERR_NOTONCHANNEL( client_, channel->_name );
			else if ( channel->_mode & CMODE_TOPIC && !( member->second & UMODE_CHANOP ) )
				client_._msg_out += ERR_CHANOPRIVSNEEDED( client_, channel->_name );
			else {
				channel->_topic = client_._message._params[ 1 ];
				channel->broadcast( RPL_TOPIC_CHANGE( client_, channel->_name, channel->_topic ) );
			}
		}
	}
}
