/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:46:37 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/28 16:24:52 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::PASS( irc::client& client_ ) {
	if ( client_._message._params.empty() ) client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
	else if ( client_._realname.compare( "*" ) )
		client_._msg_out += ERR_ALREADYREGISTRED( client_ );
	else if ( client_._message._params.front().compare( irc::server::__password ) )
		client_._msg_out += ERR_PASSWDMISMATCH( client_ );
	else
		return;
	client_._quit = true;
}
