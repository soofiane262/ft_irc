/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:22:19 by mel-hous          #+#    #+#             */
/*   Updated: 2023/03/30 14:53:19 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

static void assignMode( irc::client& client_, char c, bool& add, irc::channel& channel,
						irc::channel::member_iterator& member_it2 ) {
	if ( client_._message._params.size() < 3 ) client_._msg_out = ERR_NEEDMOREPARAMS( client_ );
	else if ( member_it2 == channel._members.end() ) {
		client_._msg_out =
			ERR_USERNOTINCHANNEL( client_, client_._message._params[ 2 ], channel._name );
	} else if ( c == 'o' && add ) {
		member_it2->second |= UMODE_CHANOP;
	} else if ( c == 'o' ) {
		member_it2->second &= ~UMODE_CHANOP;
	} else if ( c == 'v' && add ) {
		member_it2->second |= UMODE_VOICE;
	} else {
		member_it2->second &= ~UMODE_VOICE;
	}
}

static void assignMode( irc::client& client_, char mode_, bool& add_, irc::channel& channel_ ) {
	if ( mode_ == 'i' && add_ ) channel_._mode |= CMODE_INVITE;
	else if ( mode_ == 'i' )
		channel_._mode &= ~CMODE_INVITE;
	else if ( mode_ == 'm' && add_ )
		channel_._mode |= CMODE_MODERATED;
	else if ( mode_ == 'm' )
		channel_._mode &= ~CMODE_MODERATED;
	else if ( mode_ == 's' && add_ )
		channel_._mode |= CMODE_SECRET;
	else if ( mode_ == 's' )
		channel_._mode &= ~CMODE_SECRET;
	else if ( mode_ == 'p' && add_ )
		channel_._mode |= CMODE_PRIVATE;
	else if ( mode_ == 'p' )
		channel_._mode &= ~CMODE_PRIVATE;
	else if ( mode_ == 'n' && add_ )
		channel_._mode |= CMODE_NOEXTERNAL;
	else if ( mode_ == 'n' )
		channel_._mode &= ~CMODE_NOEXTERNAL;
	else if ( mode_ == 't' && add_ )
		channel_._mode |= CMODE_TOPIC;
	else if ( mode_ == 't' )
		channel_._mode &= ~CMODE_TOPIC;
	else if ( mode_ == 'k' && add_ ) {
		if ( channel_._mode & CMODE_KEY ) client_._msg_out += ERR_KEYSET( client_, channel_._name );
		else if ( client_._message._params.size() > 2 && !client_._message._params[ 2 ].empty() ) {
			channel_._key = client_._message._params[ 2 ];
			client_._message._params.erase( client_._message._params.begin() + 2 );
			channel_._mode |= CMODE_KEY;
		}
	} else if ( mode_ == 'k' )
		channel_._mode &= ~CMODE_KEY;
	else if ( mode_ == 'l' && add_ ) {
		if ( client_._message._params.size() < 3 )
			client_._msg_out += ERR_MODELIMITNEEDMOREPARAMS( client_ );
		else {
			int limit = irc::utils::ft_stoi( client_._message._params[ 2 ] );
			client_._message._params.erase( client_._message._params.begin() + 2 );
			if ( limit < 0 ) return;
			channel_._limit = std::min( limit, MAX_FD );
			channel_._mode |= CMODE_LIMIT;
		}
	} else if ( mode_ == 'l' )
		channel_._mode &= ~CMODE_LIMIT;
	else
		client_._msg_out += ERR_UNKNOWNMODE( client_, mode_ );
}

void irc::commands::MODE( irc::client& client_ ) {
	if ( client_._message._params.empty() || client_._message._params.front().empty() ) {
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
		return;
	}
	channel* channel = irc::server::__serv->findChannel( client_._message._params[ 0 ] );
	if ( channel == NULL ) {
		client_._msg_out += ERR_NOSUCHCHANNEL( client_, client_._message._params[ 0 ] );
		return;
	}
	if ( client_._message._params.size() == 1 ) {
		client_._msg_out += RPL_CHANNELMODEIS( client_, channel->_name, channel->getModes() );
		return;
	}
	irc::channel::member_iterator member_it = channel->_members.find( &client_ ), member_it2;
	if ( client_._message._params.size() >= 3 )
		member_it2 =
			channel->getMember( irc::server::__serv->findClient( client_._message._params[ 2 ] ) );
	if ( member_it == channel->_members.end() ) {
		client_._msg_out += ERR_NOTONCHANNEL( client_, channel->_name );
		return;
	} else if ( !( member_it->second & UMODE_CHANOP ) ) {
		client_._msg_out += ERR_CHANOPRIVSNEEDED( client_, channel->_name );
		return;
	}
	bool add = true;
	for ( std::string::const_iterator it = client_._message._params[ 1 ].begin();
		  it != client_._message._params[ 1 ].end(); ++it ) {
		if ( *it == '+' ) add = true;
		else if ( *it == '-' )
			add = false;
		else if ( std::strchr( UMODES_CHAN, *it ) != NULL )
			assignMode( client_, *it, add, *channel, member_it2 );
		else
			assignMode( client_, *it, add, *channel );
	};
	client_._msg_out += RPL_CHANNELMODEIS( client_, channel->_name, channel->getModes() );
}
