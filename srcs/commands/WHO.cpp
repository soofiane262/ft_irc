/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 09:09:36 by mel-hous          #+#    #+#             */
/*   Updated: 2023/03/31 10:58:33 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"


void irc::commands::WHO( irc::client& client_ ){
    if (client_._message._params.size() < 1){
        client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
        return ;
    }
    
}