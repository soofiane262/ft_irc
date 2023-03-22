/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NOTICE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acmaghou <acmaghou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 13:43:28 by acmaghou          #+#    #+#             */
/*   Updated: 2023/03/22 14:13:33 by acmaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void	irc::commands::NOTICE( irc::client& client_ ) {
	if (client_._message._params.size() > 2) {
		if (client_._message._params.front().at(0) == '#') {
			irc::channel*	chan = irc::server::__serv->findChannel(client_._message._params.front() );
			if (chan) {
				for ( irc::channel::member_iterator it = chan->_members.begin();
				  it != chan->_members.end(); ++it )
				if ( ( *it )->_nickname != client_._nickname )
					( *it )->_msg_out += MSG( client_, chan->_name, client_._message._params.back());
			}
		}
		else {
			irc::client* user = irc::server::__serv->findClient( client_._message._params.front() );
			if (user)
				user->_msg_out += MSG( client_, user->_nickname, client_._message._params.back() );
		}
	}
}