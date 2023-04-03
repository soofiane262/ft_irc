/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:02:42 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/03 15:17:15 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::channel::broadcast( std::string msg_ ) {
	irc::channel::member_iterator member_it;
	for ( member_it = this->_members.begin(); member_it != this->_members.end(); ++member_it )
		( *member_it ).first->_msg_out += msg_;
} // broadcast

void irc::channel::broadcast( irc::client& client_, std::string msg_ ) {
	irc::channel::member_iterator member_it;
	for ( member_it = this->_members.begin(); member_it != this->_members.end(); ++member_it )
		if ( client_._nickname.compare( member_it->first->_nickname ) )
			member_it->first->_msg_out += msg_;
} // broadcast

bool irc::channel::isMember( irc::client* member ) {
	irc::channel::member_iterator member_it;
	member_it = _members.find( member );
	return ( member_it != _members.end() );
} // isMember

bool irc::channel::addMember( irc::client* client_, std::string& key_ ) {
	irc::client& client = *client_;
	if ( this->_members.empty() ) goto add;
	if ( this->_mode & CMODE_LIMIT &&
		 static_cast< unsigned short >( this->_members.size() ) >= this->_limit ) {
		client_->_msg_out += ERR_CHANNELISFULL( client, this->_name );
		return false;
	}
	if ( this->_mode & CMODE_KEY && this->_key != key_ ) {
		client_->_msg_out += ERR_BADCHANNELKEY( client, this->_name );
		return false;
	}
	if ( this->_mode & ( CMODE_INVITE | CMODE_PRIVATE | CMODE_SECRET ) &&
		 !client_->isInvited( this->_name ) ) {
		client_->_msg_out += ERR_INVITEONLYCHAN( client, this->_name );
		return false;
	}
add:
	std::pair< irc::channel::member_iterator, bool > it =
		this->_members.insert( std::make_pair( client_, '\0' ) );
	if ( this->_members.size() == 1 ) it.first->second = UMODE_CHANOP | UMODE_CHANOWNER;
	client.joinChannel( this->_name );
	return true;
} // addMember

std::string irc::channel::getMembers( void ) {
	std::string					  members;
	irc::channel::member_iterator member_it;
	for ( member_it = this->_members.begin(); member_it != this->_members.end(); ++member_it )
		members +=
			( member_it->second & UMODE_CHANOP ? "@" :
												 ( member_it->second & UMODE_VOICE ? "+" : "" ) ) +
			( *member_it ).first->_nickname + ' ';
	return members;
} // getMembers

std::string irc::channel::getModes( void ) {
	std::string modes = "+";
	if ( this->_mode & CMODE_INVITE ) modes += 'i';
	if ( this->_mode & CMODE_MODERATED ) modes += 'm';
	if ( this->_mode & CMODE_NOEXTERNAL ) modes += 'n';
	if ( this->_mode & CMODE_SECRET ) modes += 's';
	if ( this->_mode & CMODE_PRIVATE ) modes += 'p';
	if ( this->_mode & CMODE_TOPIC ) modes += 't';
	if ( this->_mode & CMODE_KEY ) modes += 'k';
	if ( this->_mode & CMODE_LIMIT ) modes += 'l';
	return modes;
} // getModes

irc::channel::member_iterator irc::channel::getMember( std::string& member_name_ ) {
	irc::channel::member_iterator member_it = _members.begin();
	while ( member_it != _members.end() && member_it->first->_nickname != member_name_ )
		member_it++;
	return ( member_it );
} // getMember

irc::channel::member_iterator irc::channel::getMember( irc::client* member_ ) {
	return ( _members.find( member_ ) );
} // getMember

irc::channel::member_iterator irc::channel::getMemberByUsername( std::string& member_name_ ) {
	irc::channel::member_iterator member_it = _members.begin();
	while ( member_it != _members.end() && member_it->first->_username != member_name_ )
		member_it++;
	return ( member_it );
} // getMemberByUsername

/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */
/*                                  Channel management                                  */
/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

irc::channel* irc::server::addChannel( std::string& channel_name_ ) {
	std::pair< irc::server::channel_iterator, bool > ch_pr =
		this->_channels.insert( std::make_pair( channel_name_, irc::channel( channel_name_ ) ) );
	ch_pr.first->second._mode = CMODE_NOEXTERNAL | CMODE_SECRET;
	return &ch_pr.first->second;
} // addChannel

irc::channel* irc::server::findChannel( std::string& channel_name_ ) {
	irc::server::channel_iterator ch_it;
	for ( ch_it = this->_channels.begin();
		  ch_it != this->_channels.end() && ch_it->second._name.compare( channel_name_ ); ch_it++ )
		continue;
	return ch_it != this->_channels.end() ? &ch_it->second : NULL;
} // findChannel

irc::server::channel_type& irc::server::getChannels( void ) {
	return this->_channels;
} // getChannels

void irc::server::removeChannel( channel& channel_ ) {
	this->_channels.erase( channel_._name );
} // removeChannel

void irc::server::execCommand( irc::client& client_ ) { this->_commands[ client_ ]; }

/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */
/*                                     Broadcasting                                     */
/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

void irc::server::broadcastJoinedChannels( irc::client& client_, std::string msg_ ) {
	irc::server::channel_iterator ch_it;
	for ( ch_it = this->_channels.begin(); ch_it != this->_channels.end(); ch_it++ ) {
		if ( client_._channels_joined.find( ch_it->first ) != client_._channels_joined.end() )
			ch_it->second.broadcast( client_, msg_ );
	}
} // broadcastJoinedChannels
