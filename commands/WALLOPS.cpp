/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WALLOPS.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 23:34:02 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/02 23:39:28 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::WALLOPS( irc::client& client_ ) {
	if ( client_._mode & UMODE_OPERATOR ) {
		if ( client_._message._params.empty() || client_._message._params.front().empty() )
			client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
		else
			irc::server::__serv->broadcastWallops( client_ );
	} else
		client_._msg_out += ERR_NOPRIVILEGES( client_ );
}
