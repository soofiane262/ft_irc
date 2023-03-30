/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:47:59 by mel-hous          #+#    #+#             */
/*   Updated: 2023/03/30 17:53:18 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::KICK( irc::client& client_ ){
    if(client_._message._params.size() < 2)
        ERR_NEEDMOREPARAMS(client_);
    std::vector<std::string> channels = irc::utils::split(client_._message._params[0], ',');
    std::vector<std::string> members = irc::utils::split(client_._message._params[1], ',');
    if(channels.size() < 2)
    {
        channel *channel_ = irc::server::__serv->findChannel(channels[0]);
        if(!channel_){
		client_._msg_out += ERR_NOSUCHCHANNEL( client_, client_._message._params[ 0 ] );
		return;
	    }
        if(channel_->isMember(&client_)){
            client_._msg_out += ERR_USERNOTINCHANNEL( client_, client_._nickname, channel_->_name );
            return ;
        }
        if(!(channel_->getMember(&client_)->second & UMODE_CHANOP)){
		client_._msg_out += ERR_CHANOPRIVSNEEDED( client_, channel_->_name );
		return;
	    }
    }
}