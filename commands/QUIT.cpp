/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:47:20 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/02 16:07:36 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::QUIT( irc::client& client_ ) {
	client_._quit = true;
	if ( !client_._message._params.empty() ) client_._away_msg = client_._message._params[ 0 ];
	else
		client_._away_msg.clear();
	client_._msg_out = RPL_CLOSINGLINK( client_ );
}
