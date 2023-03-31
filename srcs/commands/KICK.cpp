/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 15:47:59 by mel-hous          #+#    #+#             */
/*   Updated: 2023/03/31 09:03:01 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

void irc::commands::KICK( irc::client& client_ ){
    if(client_._message._params.size() < 2){
       client_._msg_out += ERR_NEEDMOREPARAMS(client_);
        return;
    }
    std::vector<std::string> channels = irc::utils::split(client_._message._params[0], ',');
    std::vector<std::string> members = irc::utils::split(client_._message._params[1], ',');
    irc::client *client;
    channel     *channel_;
    for(size_t i = 0; i < channels.size(); i++)
    {
        channel_ = irc::server::__serv->findChannel(channels[i]);
        if(!channel_){
		    client_._msg_out += ERR_NOSUCHCHANNEL( client_, channels[i] );
		    return;
	    }
        if(!(channel_->isMember(&client_))){
            client_._msg_out += ERR_USERNOTINCHANNEL( client_, client_._nickname, channel_->_name );
            return ;
        }
        if(!(channel_->getMember(&client_)->second & UMODE_CHANOP)){
		    client_._msg_out += ERR_CHANOPRIVSNEEDED( client_, channel_->_name );
		    return;
	    }
        for(size_t j = 0; j < members.size() ; j++)
        {
            client = channel_->getMember(members[j])->first;
            if(!client)
                return ;
            if(!(channel_->isMember(client))){
		        // client_._msg_out += ERR_USERNOTINCHANNEL( client_, client->_nickname, channel_->_name);
		        return;
	        }
            if (client->_nickname != client_._nickname)
            {
                client->_message._params.assign(1,channel_->_name);
                irc::commands::PART(*client);
            }
        }
        
    }
}