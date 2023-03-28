/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:22:19 by mel-hous          #+#    #+#             */
/*   Updated: 2023/03/28 17:44:38 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::MODE( irc::client& client_ ) {
	if ( client_._message._params.size() < 2 || client_._message._params.front().empty() ) {
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
	} else {
		irc::channel* channel =
			irc::server::__serv->findChannel( client_._message._params.front() );
		if ( channel == NULL ) {
			client_._msg_out += ERR_NOSUCHCHANNEL( client_, client_._message._params.front() );
		} else {
			if ( client_._message._params.size() == 2 ) {
				client_._msg_out +=
					RPL_CHANNELMODEIS( client_, channel->_name, channel->getModes() );
			} else {
				// addModes
			}
		}
	}
}
