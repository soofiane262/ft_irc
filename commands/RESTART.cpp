/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RESTART.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 12:51:53 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/02 13:48:47 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::RESTART( irc::client& client_ ) {
	if ( client_._mode & UMODE_OPERATOR ) {
		client_._msg_out	   = RPL_RESTART( client_ );
		irc::server::__restart = true;
	} else
		client_._msg_out += ERR_NOPRIVILEGES( client_ );
}
