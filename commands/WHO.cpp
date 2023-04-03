/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 09:09:36 by mel-hous          #+#    #+#             */
/*   Updated: 2023/04/03 16:03:45 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

bool isMatch( irc::client::channel_type firstchannels, irc::client::channel_type secondchannels ) {
	irc::client::channel_iterator it_1, it_2;
	if ( firstchannels.size() == 0 || secondchannels.size() == 0 ) return ( true );
	for ( it_1 = firstchannels.begin(), it_2 = secondchannels.begin();
		  it_1 != firstchannels.end() && it_2 != secondchannels.end(); it_1++, it_2++ ) {
		if ( *it_1 == *it_2 ) return ( false );
	}
	return ( true );
}

void memberChannels( irc::client& client_, irc::client& member ) {
	std::string channel_name = "*";
	if ( member._channels_joined.size() == 0 ) {
		client_._msg_out += RPL_WHOREPLYNAME( client_, channel_name, member );
		return;
	}
	for ( irc::client::channel_iterator it = member._channels_joined.begin();
		  it != member._channels_joined.end(); it++ ) {
		channel_name = *it;
		client_._msg_out += RPL_WHOREPLYNAME( client_, channel_name, member );
	}
}

void irc::commands::WHO( irc::client& client_ ) {
	irc::server::client_iterator cl_it;
	if ( client_._message._params.size() >= 1 && !client_._message._params.front().empty() ) {
		std::string name = client_._message._params.front();
		if ( name[ 0 ] == '#' ) {
			name				  = irc::utils::toLower( client_._message._params.front() );
			irc::channel* channel = NULL;
			if ( client_.isInChannel( name ) ) channel = irc::server::__serv->findChannel( name );
			bool oper;
			if ( channel ) {
				if ( client_._mode & UMODE_OPERATOR ||
					 channel->getMember( &client_ )->second & UMODE_CHANOP )
					oper = true;
				for ( irc::channel::member_iterator it = channel->_members.begin();
					  it != channel->_members.end(); it++ ) {
					if ( oper ) client_._msg_out += RPL_WHOREPLY_PTR( client_, channel, it->first );
					else {
						if ( !( it->first->_mode & UMODE_INVISIBLE ) )
							client_._msg_out += RPL_WHOREPLY_PTR( client_, channel, it->first );
					}
				}
			}
			client_._msg_out += RPL_ENDOFWHO( client_, client_._message._params.front() );
		} else {
			irc::server::client_type clients = irc::server::__serv->getClients();
			if ( name == "0" ) {
				bool oper = false, matchingChannels = false;
				if ( client_._mode & UMODE_OPERATOR ) oper = true;
				for ( cl_it = clients.begin(); cl_it != clients.end(); cl_it++ ) {
					matchingChannels =
						isMatch( client_._channels_joined, cl_it->second._channels_joined );
					if ( cl_it->second._nickname == client_._nickname ||
						 ( oper && matchingChannels ) ||
						 ( matchingChannels && !( cl_it->second._mode & UMODE_INVISIBLE ) ) )
						memberChannels( client_, cl_it->second );
				}
				client_._msg_out += RPL_ENDOFWHO( client_, "*" );
			} else {
				for ( cl_it = clients.begin(); cl_it != clients.end(); cl_it++ )
					if ( cl_it->second._nickname == name ) memberChannels( client_, cl_it->second );
				client_._msg_out += RPL_ENDOFWHO( client_, name );
			}
		}
	} else
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
}
