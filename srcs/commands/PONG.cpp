/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PONG.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:44:07 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/20 16:57:55 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::PONG( irc::client& client_ ) {
	if ( client_._message._params.empty() ) client_._msg_out = ERR_NOORIGIN( client_ );
	else if ( client_._message._params.front().compare( irc::server::__hostaddr ) )
		client_._msg_out = ERR_NOSUCHSERVER( client_, client_._message._params.front() );
}
