/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 13:32:25 by mel-hous          #+#    #+#             */
/*   Updated: 2023/03/29 12:28:21 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"irc.hpp"

bool irc::client::hasmodeOP(irc::channel& channel){
    return(channel.getMemberValue(this)->second & UMODE_CHANOP);
};