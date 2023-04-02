/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OPER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 12:57:04 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/31 15:02:25 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::OPER( irc::client& client_ ) {
	if ( client_._message._params.size() < 2 ) client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
	else if ( client_._message._params[ 0 ].compare( client_._nickname ) )
		return;
	else if ( client_._message._params[ 1 ].compare( irc::server::__operpass ) ) {
		client_._msg_out += ERR_PASSWDMISMATCH( client_ );
	} else if ( !( client_._mode & UMODE_OPERATOR ) ) {
		client_._mode |= UMODE_OPERATOR;
		client_._msg_out += RPL_YOUREOPER( client_ );
	}
}
