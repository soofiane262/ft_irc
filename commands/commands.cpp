/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acmaghou <acmaghou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 12:13:09 by acmaghou          #+#    #+#             */
/*   Updated: 2023/03/09 14:44:26 by acmaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

irc::commands::commands(std::string command, std::string argument) : cmd(command), arg(argument) {
}

irc::commands::~commands() {
	
}

// creating a channel/ if channel does not exist, the user creating the channel will be first to join the channel which makes you an operator

void	irc::commands::join(const irc::user &x, const std::string &msg) {
	
}

void	irc::commands::part(const irc::user &x, const std::string &message) {
	
}
void	irc::commands::msg(const irc::user &x, const std::string &message) {
	
}
void	irc::commands::whois(const irc::user &x, const std::string &message) {

}