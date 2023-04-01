/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:02:42 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/31 22:12:37 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

int irc::channel::addMember( irc::client* client_, std::string& key_ ) {
	irc::client& client = *client_;
	if ( this->_mode & CMODE_LIMIT &&
		 static_cast< unsigned short >( this->_members.size() ) >= this->_limit ) {
		client_->_msg_out += ERR_CHANNELISFULL( client, this->_name );
		return -1;
	}
	if ( this->_mode & CMODE_KEY && this->_key != key_ ) {
		client_->_msg_out += ERR_BADCHANNELKEY( client, this->_name );
		return -1;
	}
	if ( this->_mode & ( CMODE_INVITE | CMODE_PRIVATE | CMODE_SECRET ) &&
		 !client_->isInvited( this->_name ) ) {
		client_->_msg_out += ERR_INVITEONLYCHAN( client, this->_name );
		return -1;
	}
	std::pair< irc::channel::member_iterator, bool > it =
		this->_members.insert( std::make_pair( client_, '\0' ) );
	if ( this->_members.size() == 1 ) it.first->second = UMODE_CHANOP | UMODE_CHANOWNER;
	client.joinChannel( this->_name );
	return it.second != 0;
}

void irc::channel::broadcast( std::string msg_ ) {
	irc::channel::member_iterator member_it;
	for ( member_it = this->_members.begin(); member_it != this->_members.end(); ++member_it )
		( *member_it ).first->_msg_out += msg_;
}

void irc::channel::broadcast( irc::client& client_, std::string msg_ ) {
	irc::channel::member_iterator member_it;
	for ( member_it = this->_members.begin(); member_it != this->_members.end(); ++member_it )
		if ( client_._nickname.compare( member_it->first->_nickname ) )
			member_it->first->_msg_out += msg_;
}

std::string irc::channel::getMembers( void ) {
	std::string					  members;
	irc::channel::member_iterator member_it;
	for ( member_it = this->_members.begin(); member_it != this->_members.end(); ++member_it )
		members +=
			( member_it->second & UMODE_CHANOP ? "@" : "" ) + ( *member_it ).first->_nickname + ' ';
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

// void irc::channel::addModes( const std::string& modes_ ) {
// 	std::string::const_iterator it;
// 	for ( it = modes_.begin(); it != modes_.end(); ++it ) {
// 		if ( *it == '+' ) {
// 			// this->_mode = 0xFFFFFFFF;
// 		} else if ( *it == '-' ) {
// 			this->_mode = 0;
// 		} else if ( *it == 'i' ) {
// 			this->_mode |= CMODE_INVITE;
// 		} else if ( *it == 'm' ) {
// 			this->_mode |= CMODE_MODERATED;
// 		} else if ( *it == 'n' ) {
// 			this->_mode |= CMODE_NOEXTERNAL;
// 		} else if ( *it == 's' ) {
// 			this->_mode |= CMODE_SECRET;
// 		} else if ( *it == 'p' ) {
// 			this->_mode |= CMODE_PRIVATE;
// 		} else if ( *it == 't' ) {
// 			this->_mode |= CMODE_TOPIC;
// 		} else if ( *it == 'k' ) {
// 			this->_mode |= CMODE_KEY;
// 		} else if ( *it == 'l' ) {
// 			this->_mode |= CMODE_LIMIT;
// 		}
// 	}
// }

// void irc::channel::setModes( const std::string& modes_ ) {
// 	std::string::const_iterator it;
// 	for ( it = modes_.begin(); it != modes_.end(); ++it ) {
// 		if ( *it == '+' ) {
// 			this->_mode = 0;
// 		} else if ( *it == '-' ) {
// 			// this->_mode = 0xFFFFFFFF;
// 		} else if ( *it == 'i' ) {
// 			this->_mode ^= CMODE_INVITE;
// 		} else if ( *it == 'm' ) {
// 			this->_mode ^= CMODE_MODERATED;
// 		} else if ( *it == 'n' ) {
// 			this->_mode ^= CMODE_NOEXTERNAL;
// 		} else if ( *it == 's' ) {
// 			this->_mode ^= CMODE_SECRET;
// 		} else if ( *it == 'p' ) {
// 			this->_mode ^= CMODE_PRIVATE;
// 		} else if ( *it == 't' ) {
// 			this->_mode ^= CMODE_TOPIC;
// 		} else if ( *it == 'k' ) {
// 			this->_mode ^= CMODE_KEY;
// 		} else if ( *it == 'l' ) {
// 			this->_mode ^= CMODE_LIMIT;
// 		}
// 	}
// }

bool irc::channel::isMember( irc::client* member ) {
	irc::channel::member_iterator member_it;
	member_it = _members.find( member );
	return ( member_it != _members.end() );
}

irc::channel::member_iterator irc::channel::getMember( std::string& member_name_ ) {
	irc::channel::member_iterator member_it = _members.begin();
	while ( member_it != _members.end() && member_it->first->_nickname != member_name_ )
		member_it++;
	return ( member_it );
}

irc::channel::member_iterator irc::channel::getMemberByUsername( std::string& member_name_ ) {
	irc::channel::member_iterator member_it = _members.begin();
	while ( member_it != _members.end() && member_it->first->_username != member_name_ )
		member_it++;
	return ( member_it );
}

irc::channel::member_iterator irc::channel::getMember( irc::client* member_ ) {
	return ( _members.find( member_ ) );
}
