/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_ops.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:03:22 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/20 17:03:47 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::server::addChannel( irc::channel* channel_ ) {
	this->_channels.insert( std::make_pair( channel_->_name, channel_ ) );
}

irc::channel* irc::server::findChannel( std::string& channel_name_ ) {
	irc::server::channel_iterator ch_it;
	for ( ch_it = this->_channels.begin();
		  ch_it != this->_channels.end() && ch_it->second->_name.compare( channel_name_ ); ch_it++ )
		continue;
	return ch_it != this->_channels.end() ? ch_it->second : NULL;
}
