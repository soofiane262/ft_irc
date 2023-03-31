/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHOIS.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acmaghou <acmaghou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 10:01:31 by acmaghou          #+#    #+#             */
/*   Updated: 2023/03/31 16:20:20 by acmaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

// WHOIS function following rfc2812
void	irc::commands::WHOIS( irc::client& client_ ) {
	if ( client_._message._params.size() < 1 ) {
		client_._msg_out += ERR_NONICKNAMEGIVEN( client_ );
		return ;
	}
	std::string		nick = client_._message._params[0];
	irc::client		*target = irc::server::__serv->findClient( nick );
	if ( target == NULL ) {
		client_._msg_out += ERR_NOSUCHNICK( client_, nick );
		return ;
	}
	client_._msg_out += RPL_WHOISUSER( client_, target );
	if (target->_mode & UMODE_AWAY)
		client_._msg_out += RPL_AWAY( client_, target->_nickname, target->_away_msg);
	if (target->_mode & UMODE_OPERATOR)
		client_._msg_out += RPL_WHOISOPERATOR( client_, target );
	if (target->_mode & UMODE_INVISIBLE)
		client_._msg_out += RPL_WHOISIDLE( client_, target );
	client_._msg_out += RPL_ENDOFWHOIS( client_, target );
}