/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:02:42 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/20 17:02:50 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

bool irc::channel::addMember( irc::client* client_ ) {
	return this->_members.insert( client_ ).second;
}

std::string irc::channel::getMembers( void ) {
	std::string					  members;
	irc::channel::member_iterator member_it;
	for ( member_it = this->_members.begin(); member_it != this->_members.end(); ++member_it )
		members.append( ( *member_it )->_nickname + ' ' );
	return members;
}
