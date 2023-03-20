/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:47:20 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/20 16:57:36 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::QUIT( irc::client& client_ ) {
	client_._pfd.revents |= POLLHUP;
	client_._msg_out = ERR_CLOSINGLINK( client_ );
}
