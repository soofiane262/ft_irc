/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DIE.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 12:54:25 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/02 13:48:17 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::DIE( irc::client& client_ ) {
	if ( client_._mode & UMODE_OPERATOR ) {
		client_._msg_out		= RPL_DIE( client_ );
		irc::server::__shutdown = true;
	} else
		client_._msg_out += ERR_NOPRIVILEGES( client_ );
}
