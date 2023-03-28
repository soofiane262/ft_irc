/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NOTICE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 13:43:28 by acmaghou          #+#    #+#             */
/*   Updated: 2023/03/28 16:56:59 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::NOTICE( irc::client& client_ ) {
	if ( client_._message._params.size() >= 2 && !client_._message._params[ 2 ].empty() ) {
		if ( !client_._message._params.front().empty() &&
			 client_._message._params.front()[ 0 ] != '#' ) {
			irc::channel* channel =
				irc::server::__serv->findChannel( client_._message._params.front() );
			if ( channel ) {
				for ( irc::channel::member_iterator it = channel->_members.begin();
					  it != channel->_members.end(); ++it )
					if ( ( *it ).first->_nickname != client_._nickname )
						( *it ).first->_msg_out +=
							NOTICE_MSG( client_, channel->_name, client_._message._params.back() );
			}
		} else {
			irc::client* user = irc::server::__serv->findClient( client_._message._params.front() );
			if ( user )
				user->_msg_out +=
					NOTICE_MSG( client_, user->_nickname, client_._message._params.back() );
		}
	}
}