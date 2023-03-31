/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 13:32:25 by mel-hous          #+#    #+#             */
/*   Updated: 2023/03/31 21:43:59 by sel-mars         ###   ########.fr       */
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

std::string irc::client::getIdleTime( void ) {
	std::stringstream ss;
	ss << std::difftime( time( NULL ), this->_idle );
	return ss.str();
}

std::string irc::client::getSignOnTime( void ) {
	std::stringstream ss;
	ss << this->_sign;
	return ss.str();
}

std::string irc::client::getChannels( void ) {
	std::string channels;
	for ( std::set< std::string >::iterator it = this->_channels_joined.begin();
		  it != this->_channels_joined.end(); it++ ) {
		channels += *it;
		channels += " ";
	}
	return channels;
}

void irc::client::inviteChannel( std::string& channel_name_ ) {
	this->_channels_invited.insert( channel_name_ );
}

void irc::client::joinChannel( std::string& channel_name_ ) {
	this->_channels_joined.insert( channel_name_ );
	this->_channels_invited.erase( channel_name_ );
}

void irc::client::partChannel( std::string& channel_name_ ) {
	this->_channels_joined.erase( channel_name_ );
}

bool irc::client::isInvited( std::string& channel_name_ ) {
	return this->_channels_invited.find( channel_name_ ) != this->_channels_invited.end();
}

bool irc::client::isInChannel( std::string& channel_name_ ) {
	return this->_channels_joined.find( channel_name_ ) != this->_channels_joined.end();
}
