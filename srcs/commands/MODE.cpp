/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:22:19 by mel-hous          #+#    #+#             */
/*   Updated: 2023/03/26 15:59:38 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void assign_mode(irc::client& client_, char c, bool add){
    switch (c)
    {
    case 'o':
        if (add == true && client_.has_mode(UMODE_OPERATOR) == false)
            client_._mode |= UMODE_OPERATOR;
        break;
    
    default:
        break;
    }
}

void irc::commands::MODE( irc::client& client_ ) {

    if (client_._message._params.size() < 2)
        return irc::utils::msg_out_creator(client_, ERR_NEEDMOREPARAMS(client_));
    channel *channel =  irc::server::__serv->findChannel(client_._message._params[0]);
    if (channel == NULL)
        return irc::utils::msg_out_creator(client_, ERR_NOSUCHCHANNEL(client_, client_._message._params[0]));
    if (!channel->getMember(&client_))
        return irc::utils::msg_out_creator(client_, ERR_NOTONCHANNEL(client_, client_._message._params[0]));
    // if ()
        // return irc::utils::msg_out_creator(client_, ERR_NOTONCHANNEL(client_, client_._message._params[0]));
    
    for (int i = 0; client_._message._params[1][i]; i++){
        bool add = true;
        if (client_._message._params[1][i] == '+')
            add = true;
        else if (client_._message._params[1][i] == '-')
            add = false;
        else {
            assign_mode(client_,client_._message._params[1][i], add);
        }
            
    };
    
}