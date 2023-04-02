/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:47:59 by mel-hous          #+#    #+#             */
/*   Updated: 2023/03/31 21:36:24 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::kick_member( irc::client& client_, std::string& member_username,
								 irc::channel* channel_, std::string& kick_commment_ ) {
	irc::channel::member_iterator member = channel_->getMember( &client_ );
	if ( member == channel_->_members.end() )
		client_._msg_out += ERR_NOTONCHANNEL( client_, channel_->_name );
	else if ( !( member->second & UMODE_CHANOP ) )
		client_._msg_out += ERR_CHANOPRIVSNEEDED( client_, channel_->_name );
	else {
		irc::channel::member_iterator client_it;
		client_it = channel_->getMemberByUsername( member_username );
		if ( client_it == channel_->_members.end() )
			client_._msg_out +=
				ERR_USERNOTINCHANNEL( client_, client_it->first->_nickname, channel_->_name );
		else if ( client_it->first->_nickname != client_._nickname ) {
			client_it->first->_message.clear();
			client_it->first->_message._params.push_back( channel_->_name );
			client_it->first->_message._params.push_back( kick_commment_ );
			irc::commands::PART( *client_it->first );
			client_it->first->_message.clear();
		}
	}
}

void irc::commands::KICK( irc::client& client_ ) {
	if ( client_._message._params.size() < 2 ) {
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
		return;
	}
	std::string kick_comment =
		client_._message._params[ 2 ].empty() ? client_._nickname : client_._message._params[ 2 ];
	std::vector< std::string > channels = irc::utils::split( client_._message._params[ 0 ], ',' );
	std::transform( channels.begin(), channels.end(), channels.begin(), irc::utils::toLower );
	std::vector< std::string > members = irc::utils::split( client_._message._params[ 1 ], ',' );
	irc::channel*			   channel;
	if ( channels.size() != 1 && channels.size() != members.size() ) return;
	else if ( channels.size() == 1 ) {
		channel = irc::server::__serv->findChannel( channels.front() );
		if ( !channel ) client_._msg_out += ERR_NOSUCHCHANNEL( client_, channels.front() );
		else {
			for ( std::vector< std::string >::iterator cl_it = members.begin();
				  cl_it != members.end(); ++cl_it )
				irc::commands::kick_member( client_, *cl_it, channel, kick_comment );
		}
	} else {
		for ( std::vector< std::string >::iterator cl_it = members.begin(),
												   ch_it = channels.begin();
			  cl_it != members.end(); ++cl_it, ++ch_it ) {
			channel = irc::server::__serv->findChannel( *ch_it );
			if ( !channel ) client_._msg_out += ERR_NOSUCHCHANNEL( client_, channels.front() );
			else
				irc::commands::kick_member( client_, *cl_it, channel, kick_comment );
		}
	}
}
