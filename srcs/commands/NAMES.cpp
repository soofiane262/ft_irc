/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NAMES.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 17:35:57 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/29 17:43:51 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

static void namesChannel( irc::client& client_, std::string& channel_name_ ) {
	irc::channel* channel = irc::server::__serv->findChannel( channel_name_ );
	if ( channel == NULL ) client_._msg_out += ERR_NOSUCHCHANNEL( client_, channel_name_ );
	else {
		std::stringstream ss;
		ss << channel->_members.size();
		client_._msg_out += RPL_NAMREPLY( client_, channel->_name, channel->getMembers() );
		client_._msg_out += RPL_ENDOFNAMES( client_, channel->_name );
	}
}

void irc::commands::NAMES( irc::client& client_ ) {
	if ( client_._message._params.empty() || client_._message._params.front().empty() ) {
		for ( irc::server::channel_iterator it = irc::server::__serv->getChannels().begin();
			  it != irc::server::__serv->getChannels().end(); ++it ) {
			if ( irc::server::__serv->getChannels().size() > 10 )
				client_._msg_out += ERR_TOOMANYMATCHESNAMES( client_, it->second->_name );
			else
				namesChannel( client_, it->second->_name );
		}
	} else {
		std::vector< std::string > channel_names =
			irc::utils::split( client_._message._params[ 1 ], ',' );
		for ( std::vector< std::string >::iterator it = channel_names.begin();
			  it != channel_names.end(); ++it )
			namesChannel( client_, *it );
	}
}
