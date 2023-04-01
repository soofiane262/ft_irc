/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_ops.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:03:22 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/31 17:55:15 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

irc::channel* irc::server::addChannel( std::string& channel_name_ ) {
	irc::channel* channel = new irc::channel( channel_name_ );
	this->_channels.insert( std::make_pair( channel_name_, channel ) );
	channel->_mode = CMODE_NOEXTERNAL | CMODE_TOPIC;
	return channel;
}

irc::channel* irc::server::findChannel( std::string& channel_name_ ) {
	irc::server::channel_iterator ch_it;
	for ( ch_it = this->_channels.begin();
		  ch_it != this->_channels.end() && ch_it->second->_name.compare( channel_name_ ); ch_it++ )
		continue;
	return ch_it != this->_channels.end() ? ch_it->second : NULL;
}

irc::server::channel_type& irc::server::getChannels( void ) { return this->_channels; }

void irc::server::removeChannel( channel& channel_ ) { this->_channels.erase( channel_._name ); }

void irc::server::broadcastJoinedChannels( irc::client& client_, std::string msg_ ) {
	irc::server::channel_iterator ch_it;
	for ( ch_it = this->_channels.begin(); ch_it != this->_channels.end(); ch_it++ ) {
		if ( client_._channels_joined.find( ch_it->first ) != client_._channels_joined.end() )
			ch_it->second->broadcast( client_, msg_ );
	}
}
