/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LIST.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 17:24:34 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/31 13:36:32 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

static void listChannel( irc::client& client_, std::string& channel_name_ ) {
	irc::channel* channel = irc::server::__serv->findChannel( channel_name_ );
	if ( channel == NULL ) client_._msg_out += ERR_NOSUCHCHANNEL( client_, channel_name_ );
	else {
		std::stringstream ss;
		ss << channel->_members.size();
		client_._msg_out += RPL_LIST( client_, channel->_name, ss.str(), channel->_topic );
	}
}

void irc::commands::LIST( irc::client& client_ ) {
	if ( client_._message._params.empty() || client_._message._params.front().empty() ) {
		if ( irc::server::__serv->getChannels().size() > 10 )
			client_._msg_out += ERR_TOOMANYMATCHES( client_ );
		for ( irc::server::channel_iterator it = irc::server::__serv->getChannels().begin();
			  it != irc::server::__serv->getChannels().end(); ++it ) {
			if ( !( it->second->_mode & CMODE_SECRET ) && !( it->second->_mode & CMODE_PRIVATE ) )
				listChannel( client_, it->second->_name );
		}
	} else {
		std::vector< std::string > channel_names =
			irc::utils::split( client_._message._params[ 0 ], ',' );
		for ( std::vector< std::string >::iterator it = channel_names.begin();
			  it != channel_names.end(); ++it )
			listChannel( client_, *it );
	}
	client_._msg_out += RPL_LISTEND( client_ );
}
