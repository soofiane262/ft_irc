/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 11:25:32 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/21 11:32:58 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::PRIVMSG( irc::client& client_ ) {
	if ( client_._message._params.empty() ) client_._msg_out += ERR_NORECIPIENT( client_ );
	else if ( client_._message._params.front().at( 0 ) != '#' )
		client_._msg_out += ERR_NOSUCHCHANNEL( client_, client_._message._params.front() );
	else {
		irc::channel* channel =
			irc::server::__serv->findChannel( client_._message._params.front() );
		if ( channel == NULL )
			client_._msg_out += ERR_NOSUCHCHANNEL( client_, client_._message._params.front() );
		else {
			for ( irc::channel::member_iterator it = channel->_members.begin();
				  it != channel->_members.end(); ++it )
				if ( ( *it )->_nickname != client_._nickname )
					( *it )->_msg_out += ":" + client_._nickname + "!" + client_._username + "@" +
										 client_._hostname + " PRIVMSG " + channel->_name + " :" +
										 client_._message._params.back() + CRLF;
		}
	}
}
