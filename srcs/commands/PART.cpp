/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acmaghou <acmaghou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:24:56 by acmaghou          #+#    #+#             */
/*   Updated: 2023/03/21 14:34:51 by acmaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void	irc::commands::PART( irc::client& client_ ) {
	if ( client_._message._params.empty() )
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
	else {
		irc::channel* channel = irc::server::__serv->findChannel( client_._message._params.front() );
		if ( channel == NULL )
			client_._msg_out += ERR_NOSUCHCHANNEL( client_, client_._message._params.front() );
		else {
			irc::channel::member_iterator it = channel->_members.find( &client_ );
			if ( it == channel->_members.end() )
				client_._msg_out += ERR_NOTONCHANNEL( client_, channel->_name );
			else {
				channel->_members.erase( it );
				for ( irc::channel::member_iterator it = channel->_members.begin();
					  it != channel->_members.end(); ++it )
					( *it )->_msg_out += PARTMSG( client_, channel->_name );
			}
		}
	}
}