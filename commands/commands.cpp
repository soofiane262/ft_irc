/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acmaghou <acmaghou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 12:13:09 by acmaghou          #+#    #+#             */
/*   Updated: 2023/03/09 17:06:20 by acmaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.hpp"

COM::commands(std::string command, std::string argument) : cmd(command), arg(argument) {
}

COM::~commands() {
	
}

// creating a channel/ if channel does not exist, the user creating the channel will be first to join the channel which makes you an operator

void	COM::join(const irc::user &x, const std::string &cmd, const channel& ch) {
	// x user creates a channel -> need channel name from parsing
	// example:
	// JOIN lol 
	// creates a #lol channel, so during parsing smth like this should be parsed:
	// firstPartOfCommand=JOIN , secondPartOfCommand=lol
}

// leaving server
void	COM::part(const irc::user &x, const std::string &msg) {
	
}

// lists informations about user

void	COM::whois(const irc::user &x, const std::string &msg) {

}

// give user a nickname or change it if it does not exist

void	COM::nick(const irc::user &x, const std::string &msg) {
	
}

void	COM::setUserName(const irc::user &x, const std::string &msg) {
	
}

void	COM::privmsg(const irc::user &x, const irc::user &targetUser, const std::string &msg) {
	
}