/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:22:19 by mel-hous          #+#    #+#             */
/*   Updated: 2023/04/03 15:49:24 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

static void assignMode( irc::client& client_, char c, bool add ) {
	if ( c == 'a' && !add ) client_._mode &= ~UMODE_AWAY;
	else if ( c == 'w' && add )
		client_._mode |= UMODE_WALLOPS;
	else if ( c == 'w' )
		client_._mode &= ~UMODE_WALLOPS;
	else if ( c == 'i' && add )
		client_._mode |= UMODE_INVISIBLE;
	else if ( c == 'i' )
		client_._mode &= ~UMODE_INVISIBLE;
	else if ( c == 'r' && add )
		client_._mode |= UMODE_RESTRICTED;
	else if ( c == 'r' )
		client_._mode &= ~UMODE_RESTRICTED;
	else if ( c == 'o' && !add )
		client_._mode &= ~UMODE_OPERATOR;
}

static bool assignUserMode( irc::client& client_, char c, bool add, irc::channel& channel,
							std::string& channel_modes_ ) {
	if ( client_._message._params.size() < 3 ) {
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
		return false;
	} else {
		irc::channel::member_iterator member_it =
			channel.getMember( client_._message._params[ 2 ] );
		if ( member_it == channel._members.end() ) {
			client_._msg_out +=
				ERR_USERNOTINCHANNEL( client_, client_._message._params[ 2 ], channel._name );
			return false;
		} else if ( c == 'o' && add )
			member_it->second |= UMODE_CHANOP;
		else if ( c == 'o' )
			member_it->second &= ~UMODE_CHANOP;
		else if ( c == 'v' && add )
			member_it->second |= UMODE_VOICE;
		else
			member_it->second &= ~UMODE_VOICE;
	}
	channel_modes_ += client_._message._params[ 2 ] + SPACE;
	client_._message._params.erase( client_._message._params.begin() + 2 );
	return true;
}

static bool assignMode( irc::client& client_, char mode_, bool add_, irc::channel& channel_,
						std::string& channel_modes_ ) {
	if ( mode_ == 'i' && add_ ) channel_._mode |= CMODE_INVITE;
	else if ( mode_ == 'i' )
		channel_._mode &= ~CMODE_INVITE;
	else if ( mode_ == 'm' && add_ )
		channel_._mode |= CMODE_MODERATED;
	else if ( mode_ == 'm' )
		channel_._mode &= ~CMODE_MODERATED;
	else if ( mode_ == 's' && add_ && !( channel_._mode & CMODE_PRIVATE ) )
		channel_._mode |= CMODE_SECRET;
	else if ( mode_ == 's' )
		channel_._mode &= ~CMODE_SECRET;
	else if ( mode_ == 'p' && add_ && !( channel_._mode & CMODE_SECRET ) )
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
			channel_modes_ += client_._message._params[ 2 ] + SPACE;
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
			channel_modes_ += client_._message._params[ 2 ] + SPACE;
			client_._message._params.erase( client_._message._params.begin() + 2 );
			if ( limit < 0 ) return true;
			channel_._limit = std::min( limit, MAX_FD );
			channel_._mode |= CMODE_LIMIT;
		}
	} else if ( mode_ == 'l' )
		channel_._mode &= ~CMODE_LIMIT;
	else {
		client_._msg_out += ERR_UNKNOWNMODE( client_, mode_ );
		return false;
	}
	return true;
}

void irc::commands::MODE( irc::client& client_ ) {
	if ( client_._message._params.empty() || client_._message._params.front().empty() ) {
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
	} else if ( client_._message._params.front()[ 0 ] == '#' ) {
		std::string	  channel_name = irc::utils::toLower( client_._message._params[ 0 ] );
		irc::channel* channel	   = NULL;
		if ( client_.isInChannel( channel_name ) )
			channel = irc::server::__serv->findChannel( channel_name );
		if ( channel == NULL ) {
			client_._msg_out += ERR_NOSUCHCHANNEL( client_, channel_name );
		} else if ( client_._message._params.size() == 1 ) {
			client_._msg_out += RPL_CHANNELMODEIS( client_, channel->_name, channel->getModes() );
		} else {
			irc::channel::member_iterator member_it = channel->_members.find( &client_ );
			if ( member_it == channel->_members.end() ) {
				client_._msg_out += ERR_NOTONCHANNEL( client_, channel->_name );
			} else if ( !( member_it->second & UMODE_CHANOP ) ) {
				client_._msg_out += ERR_CHANOPRIVSNEEDED( client_, channel->_name );
			} else {
				bool		add = true;
				std::string channel_modes[ 2 ];
				if ( client_._message._params[ 1 ].front() != '-' ) channel_modes[ 0 ] += '+';
				for ( std::string::const_iterator it = client_._message._params[ 1 ].begin();
					  it != client_._message._params[ 1 ].end(); ++it ) {
					if ( *it == '+' ) {
						if ( !add ) {
							add = true;
							channel_modes[ 0 ] += '+';
						}
					} else if ( *it == '-' ) {
						if ( add ) {
							add = false;
							channel_modes[ 0 ] += '-';
						}
					} else if ( std::strchr( UMODES_CHAN, *it ) != NULL &&
								assignUserMode( client_, *it, add, *channel, channel_modes[ 1 ] ) )
						channel_modes[ 0 ] += *it;
					else if ( std::strchr( UMODES_CHAN, *it ) == NULL &&
							  assignMode( client_, *it, add, *channel, channel_modes[ 1 ] ) )
						channel_modes[ 0 ] += *it;
				};
				channel_modes[ 0 ] += SPACE + channel_modes[ 1 ];
				channel->broadcast( RPL_CHANMODE( client_, channel->_name, channel_modes[ 0 ] ) );
			}
		}
	} else {
		if ( client_._message._params.size() == 1 &&
			 !client_._message._params.front().compare( client_._nickname ) ) {
			client_._msg_out += RPL_UMODEIS( client_ );
		} else if ( client_._message._params.front().compare( client_._nickname ) ) {
			client_._msg_out += ERR_USERSDONTMATCH( client_ );
		} else {
			bool		add = true;
			std::string user_modes;
			if ( client_._message._params[ 1 ].front() != '-' ) user_modes += '+';
			for ( std::string::const_iterator it = client_._message._params[ 1 ].begin();
				  it != client_._message._params[ 1 ].end(); ++it ) {
				if ( *it == '+' ) {
					if ( add ) {
						add = true;
						user_modes += '+';
					}
				} else if ( *it == '-' ) {
					if ( !add ) {
						add = false;
						user_modes += '-';
					}
				} else if ( std::strchr( UMODES_AVAIL, *it ) != NULL ) {
					assignMode( client_, *it, add );
				} else {
					client_._msg_out += ERR_UNKNOWNMODE( client_, *it );
				}
			}
			client_._msg_out += RPL_UMODE( client_ );
		}
	}
}
