/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AWAY.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 17:07:47 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/31 18:18:36 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::AWAY( irc::client& client_ ) {
	client_._idle = std::time( NULL );
	if ( client_._message._params.size() < 1 ) {
		client_._msg_out += RPL_UNAWAY( client_ );
		client_._mode &= ~UMODE_AWAY;
		client_._away_msg.clear();
		return;
	}
	client_._msg_out += RPL_NOWAWAY( client_ );
	client_._mode |= UMODE_AWAY;
	client_._away_msg = client_._message._params[ 0 ];
}
