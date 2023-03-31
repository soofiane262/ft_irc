/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:46:54 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/31 12:53:22 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::NICK( irc::client& client_ ) {
	std::time_t time_elapsed =
		std::difftime( std::time( NULL ), client_._nick_change + ( client_._nick_change == -1 ) );
	if ( client_._mode & UMODE_RESTRICTED ) ERR_RESTRICTED( client_ );
	else if ( client_._message._params.empty() || client_._message._params.front().empty() )
		client_._msg_out += ERR_NONICKNAMEGIVEN( client_ );
	else if ( !irc::utils::nickIsValid( client_._message._params.front() ) )
		client_._msg_out += ERR_ERRONEUSNICKNAME( client_ );
	else if ( irc::server::__serv->findClientByNick( client_._message._params.front() ) )
		client_._msg_out += client_._realname.compare( "*" ) ? ERR_NICKNAMEINUSE( client_ ) :
															   ERR_NICKCOLLISION( client_ );
	else if ( client_._nick_change != -1 && time_elapsed < NICK_DELAY ) {
		std::stringstream ss;
		ss << ( NICK_DELAY - time_elapsed );
		client_._msg_out += ERR_UNAVAILRESOURCE( client_, ss.str() );
	} else {
		client_._nickname	 = client_._message._params.front();
		client_._nick_change = std::time( NULL );
		return;
	}
	if ( !client_._realname.compare( "*" ) ) client_._quit = true;
}
