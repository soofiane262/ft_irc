/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:02:42 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/28 17:35:57 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

bool irc::channel::addMember( irc::client* client_ ) {
	std::pair< irc::channel::member_iterator, bool > it =
		this->_members.insert( std::make_pair( client_, '\0' ) );
	if ( !this->_mode ) it.first->second = UMODE_CHANOP | UMODE_CHANOWNER;
	return it.second;
}

std::string irc::channel::getMembers( void ) {
	std::string					  members;
	irc::channel::member_iterator member_it;
	for ( member_it = this->_members.begin(); member_it != this->_members.end(); ++member_it )
		members.append( ( *member_it ).first->_nickname + ' ' );
	return members;
}

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
}

bool irc::channel::getMember( irc::client* member ) {
	irc::channel::member_iterator member_it;
	member_it = _members.find( member );
	return ( member_it != _members.end() );
}
irc::channel::member_iterator irc::channel::getMemberValue( irc::client* member ) {
	irc::channel::member_iterator member_it;
	member_it = _members.find( member );
	return ( member_it );
}
