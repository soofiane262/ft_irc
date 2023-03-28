/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:42:15 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/28 16:08:03 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::USER( irc::client& client_ ) {
	if ( client_._message._params.size() < 4 ) client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
	else if ( client_._realname.compare( "*" ) )
		client_._msg_out += ERR_ALREADYREGISTRED( client_ );
	else {
		client_._username = client_._message._params[ 0 ];
		client_._realname = client_._message._params[ 3 ];
		std::stringstream ss;
		int				  i;
		ss << client_._message._params[ 1 ];
		ss >> i;
		client_._mode = irc::utils::intToMode( i );
		client_._msg_out += REGISTRATION_SUCCESS( client_ );
		client_._msg_out += RPL_LUSERCLIENT( client_ );
		client_._msg_out += RPL_LUSEROP( client_ );
		client_._msg_out += RPL_LUSERUNKNOWN( client_ );
		client_._msg_out += RPL_LUSERCHANNELS( client_ );
		client_._msg_out += RPL_LUSERME( client_ );
		client_._msg_out += RPL_MOTDSTART( client_ );
		client_._msg_out += RPL_MOTD( client_, "" );
		client_._msg_out +=
			RPL_MOTD( client_, "Greetings and welcome to our Internet Relay Chat server !" );
		client_._msg_out += RPL_MOTD( client_, "\n" );
		client_._msg_out += RPL_MOTD( client_, "We hope you feel at home here." );
		client_._msg_out += RPL_ENDOFMOTD( client_ );
		return;
	}
	if ( !client_._realname.compare( "*" ) ) client_._quit = true;
}
