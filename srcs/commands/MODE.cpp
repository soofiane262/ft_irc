/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:22:19 by mel-hous          #+#    #+#             */
/*   Updated: 2023/03/28 10:46:14 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void assign_mode(irc::client& client_, char c, bool add, irc::channel& channel){
    switch (c)
    {
    case 'o':
        if (add == true)
        {
            channel.getMemberValue(&client_)->second |= UMODE_CHANOP;
            client_._msg_out =  MODEMSG(client_,channel,"o","+");
        }
        else
           channel.getMemberValue(&client_)->second &= ~1;
        break;
    case 'v':
         if (add == true)
          channel.getMemberValue(&client_)->second |= UMODE_VOICE;
         else
            channel.getMemberValue(&client_)->second &= ~(1 << 1);
    case 'i':
         if (add == true)
            channel._mode |= CMODE_INVITE;
         else
            channel._mode &= ~(1 << 0);
    case 'm':
         if (add == true)
            channel._mode |= CMODE_MODERATED;
         else
            channel._mode &= ~(1 << 1);
    case 's':
         if (add == true)
            channel._mode |= CMODE_SECRET;
         else
            channel._mode &= ~(1 << 3);
    case 'p':
         if (add == true)
            channel._mode |= CMODE_PRIVATE;
         else
            channel._mode &= ~(1 << 4);
    case 't':
         if (add == true)
            channel._mode |= CMODE_TOPIC;
         else
            channel._mode &= ~(1 << 5);
    default:
        break;
    }
}

void irc::commands::MODE( irc::client& client_ ) {

    channel *channel;
    if (client_._message._params.size() < 2){
        client_._msg_out = ERR_NEEDMOREPARAMS(client_);
        return ;}
    else if ( client_._message._params[0].find('#') != std::string::npos )
    {
        channel =  irc::server::__serv->findChannel(client_._message._params[0]);
        std::cout<< channel->_name<<std::endl;
        if (channel == NULL){
            client_._msg_out = ERR_NOSUCHCHANNEL(client_, client_._message._params[0]);
            return ;
        }
        if (!channel->getMember(&client_)){
            client_._msg_out = ERR_NOTONCHANNEL(client_, client_._message._params[0]);
            return ;
        }
    }
    else{
        channel =  irc::server::__serv->findChannel(client_.last_channel);
        std::cout<< channel->_name<<std::endl;
        if (channel == NULL){
            client_._msg_out = ERR_NOSUCHCHANNEL(client_, client_._message._params[0]);
            return ;
        }
        if (!channel->getMember(&client_)){
            client_._msg_out = ERR_NOTONCHANNEL(client_, client_._message._params[0]);
            return ;
        }
    }
    // if ()
    //     return irc::utils::msg_out_creator(client_, ERR_NOTONCHANNEL(client_, client_._message._params[0]));
    bool add = true;
    for (int i = 0; client_._message._params[1][i]; i++){
        if (client_._message._params[1][i] == '+')
            add = true;
        else if (client_._message._params[1][i] == '-')
            add = false;
        else {
            assign_mode(client_,client_._message._params[1][i], add, *channel);
        }
            
    };
    
}