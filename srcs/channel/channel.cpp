/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:02:42 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/28 17:09:58 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

bool irc::channel::addMember( irc::client* client_ ) {
	return this->_members.insert( std::make_pair( client_, '\0' ) ).second;
}

std::string irc::channel::getMembers( void ) {
	std::string					  members;
	irc::channel::member_iterator member_it;
	for ( member_it = this->_members.begin(); member_it != this->_members.end(); ++member_it )
		members.append( ( *member_it ).first->_nickname + ' ' );
	return members;
}

bool irc::channel::getMember( irc::client* member) {
	irc::channel::member_iterator member_it;
	member_it = _members.find(member);
	return (member_it != _members.end());
}
irc::channel::member_iterator irc::channel::getMemberValue( irc::client* member) {
	irc::channel::member_iterator member_it;
	member_it = _members.find(member);
	return(member_it);
}

irc::channel::member_iterator irc::channel::getMember( std::string member) {
	irc::channel::member_iterator member_it;
	member_it = _members.begin();
	while(member_it != _members.end())
	{
		if (member_it->first->_nickname == member)
			break;
		member_it++;
	}
	return(member_it);
}
