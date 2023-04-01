/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WHO.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 09:09:36 by mel-hous          #+#    #+#             */
/*   Updated: 2023/04/01 08:54:39 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"


bool isMatch( irc::client::channel_type firstchannels, irc::client::channel_type secondchannels)
{
    irc::client::channel_iterator it_1,it_2;
    if(firstchannels.size()== 0 || secondchannels.size() == 0)
        return(true);
    for(it_1 = firstchannels.begin(),it_2 = secondchannels.begin();
         it_1 != firstchannels.end() && it_2 != secondchannels.end(); it_1++ ,it_2++)
         {
            if(*it_1 == *it_2)
                return(false);
         }
    return(true);
}

void irc::commands::WHO( irc::client& client_ ){
    irc::server::client_iterator cl_it;
    if (client_._message._params.size() >= 1 && client_._message._params.front() != "0"){
        if(client_._message._params.front()[0] == '#')
        {
            irc::channel *channel = irc::server::__serv->findChannel(client_._message._params.front());
            if(!channel) return;
            bool oper;
            if (client_._mode & UMODE_OPERATOR || channel->getMember(&client_)->second & UMODE_CHANOP)
                oper = true;
            for(irc::channel::member_iterator it = channel->_members.begin(); it != channel->_members.end(); it++)
            {
                if(oper)
                {
                    
                }
                else
                {
                    if(!(it->first->_mode & UMODE_INVISIBLE))
                    {
                        
                    }
                }
            }
            
        }
        else{
            
        }
    }
    else{
        irc::server::client_type clients = irc::server::__serv->getClients();
        bool oper;
        if (client_._mode & UMODE_OPERATOR)
            oper = true;
        for(cl_it = clients.begin(); cl_it != clients.end(); cl_it++)
        {
            if(oper)
            {
                if (isMatch(client_._channels_joined,cl_it->second._channels_joined))
                   std::cout<< ':' << irc::server::__hostaddr << " 352 " << client_._nickname << ' ' << cl_it->second._username << ' '<< cl_it->second._hostname << ' '<< cl_it->second._nickname<< ' ' << cl_it->second._realname << "\r\n"<<std::endl;
            }
            else
            {
                if (isMatch(client_._channels_joined,cl_it->second._channels_joined) && !(cl_it->second._mode & UMODE_INVISIBLE))
                        std::cout<< ':' << irc::server::__hostaddr << " 352 " << client_._nickname << ' ' << cl_it->second._username << ' '<< cl_it->second._hostname << ' '<< cl_it->second._nickname<< ' ' << cl_it->second._realname << "\r\n"<<std::endl;

            }
        }
    }
    
}