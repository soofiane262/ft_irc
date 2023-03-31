/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 13:32:25 by mel-hous          #+#    #+#             */
/*   Updated: 2023/03/31 16:08:35 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

std::string irc::client::getModes( void ) {
	std::string modes = "+";
	if ( _mode & UMODE_AWAY ) modes += "a";
	if ( _mode & UMODE_WALLOPS ) modes += "w";
	if ( _mode & UMODE_INVISIBLE ) modes += "i";
	if ( _mode & UMODE_RESTRICTED ) modes += "r";
	if ( _mode & UMODE_OPERATOR ) modes += "o";
	return modes;
}

void irc::client::inviteChannel( std::string& channel_name_ ) {
	this->_channels_invited.insert( channel_name_ );
}

void irc::client::joinChannel( std::string& channel_name_ ) {
	this->_channels_joined.insert( channel_name_ );
	this->_channels_invited.erase( channel_name_ );
}

bool irc::client::isInvited( std::string& channel_name_ ) {
	return this->_channels_invited.find( channel_name_ ) != this->_channels_invited.end();
}

bool irc::client::isInChannel( std::string& channel_name_ ) {
	return this->_channels_joined.find( channel_name_ ) != this->_channels_joined.end();
}
