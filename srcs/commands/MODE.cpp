/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 15:22:19 by mel-hous          #+#    #+#             */
/*   Updated: 2023/03/29 12:52:48 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"


void assign_mode_member(irc::client& client_, char c, bool add, irc::channel& channel){

    if (client_._message._params.size() >= 3)
    {
        if (channel.getMember(client_._message._params[2]) == channel._members.end()){
            client_._msg_out = ERR_NOTONCHANNEL(client_, client_._message._params[0]);
            return ;
        }
        switch (c)
        {
        case 'o':
            if (add == true)
            {
                channel.getMember(client_._message._params[2])->second |= UMODE_CHANOP;
                client_._msg_out +=  MODEMSG(client_,channel,"o","+");
            }
            else{
                channel.getMember(client_._message._params[2])->second &= ~UMODE_CHANOP;
                client_._msg_out +=  MODEMSG(client_,channel,"o","-");  
            }
            break;
        case 'v':
             if (add == true){
                channel.getMember(client_._message._params[2])->second |= UMODE_VOICE;
                client_._msg_out +=  MODEMSG(client_,channel,"v","+");
             }
             else{
                channel.getMember(client_._message._params[2])->second &= ~UMODE_VOICE;
                client_._msg_out +=  MODEMSG(client_,channel,"v","-");
             }
            default:
                break;
        }
    }
}  
void assign_mode_channel(irc::client& client_, char c, bool add, irc::channel& channel){
    
    switch (c)
    {
        case 'i':
             if (add == true)
                channel._mode |= CMODE_INVITE;
             else
                channel._mode &= ~CMODE_INVITE;
            break;
        case 'm':
             if (add == true)
                channel._mode |= CMODE_MODERATED;
             else
                channel._mode &= ~CMODE_MODERATED;
            break;
        case 's':
             if (add == true)
                channel._mode |= CMODE_SECRET;
             else
                channel._mode &= ~CMODE_SECRET;
            break;
        case 'p':
             if (add == true)
                channel._mode |= CMODE_PRIVATE;
             else
                channel._mode &= ~CMODE_PRIVATE;
            break;
        case 'n':
             if (add == true)
                channel._mode |= CMODE_NOEXTERNAL;
             else
                channel._mode &= ~CMODE_NOEXTERNAL;
            break;
        case 't':
             if (add == true)
                channel._mode |= CMODE_TOPIC;
             else
                channel._mode &= ~CMODE_TOPIC;
            break;
        case 'k':
             if (add == true && client_._message._params.size() > 3)
             {
                channel._key = client_._message._params[3];
                client_._message._params.erase(client_._message._params.begin() + 3);
                channel._mode |= CMODE_KEYLOCK;
             }
             else
                channel._mode &=  CMODE_KEYLOCK;
            break;
        case 'l':
             if (add == true  && client_._message._params.size() > 3)
             {
                try{
                    channel.limit = std::stoi(client_._message._params[3]);
                    client_._message._params.erase(client_._message._params.begin() + 3);
                }
                catch(...){
                    return ;
                }
                channel._mode |= CMODE_TOPIC;
                
             }
             else
                channel._mode &= ~CMODE_TOPIC;
            break;
        default:
            break;
    }
}

void irc::commands::MODE( irc::client& client_ ) {
      
    if (client_._message._params.size() < 1 || client_._message._params.front().empty()){
        client_._msg_out = ERR_NEEDMOREPARAMS(client_);
        return ;
    }
    channel *channel =  irc::server::__serv->findChannel(client_._message._params[0]);
    if (channel == NULL){
        client_._msg_out = ERR_NOSUCHCHANNEL(client_, client_._message._params[0]);
        return ;
    }
    // if(client_.hasmodeOP(*channel))
    //     std::cout<< "!!!!!!!@@@@######$$$$$$"<<std::endl;
    // std::cout<< client_._message._params.size()<<std::endl;
    if (client_._message._params.size() == 1)
    {
        // client_._msg_out += RPL_CHANNELMODEIS( client_, channel->_name, channel->getModes() );
        return ;
    }
    if ( client_._message._params[0].find('#') != std::string::npos )
    {
        
        std::string s = "ov";
        bool add = true;
        for (int i = 0; client_._message._params[1][i]; i++){
            if (client_._message._params[1][i] == '+')
                add = true;
            else if (client_._message._params[1][i] == '-')
                add = false;
            else if (s.find(client_._message._params[1][i]) != std::string::npos)
                assign_mode_member(client_,client_._message._params[1][i], add, *channel);
            else {
                assign_mode_channel(client_,client_._message._params[1][i], add, *channel);
            }    
        };
    }
}