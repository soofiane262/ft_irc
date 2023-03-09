/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acmaghou <acmaghou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/09 11:54:34 by acmaghou          #+#    #+#             */
/*   Updated: 2023/03/09 14:44:11 by acmaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDS_HPP
# define COMMANDS_HPP

#include "../user/user.hpp"

#include <csignal>		// signal
#include <cstddef>		// size_t
#include <exception>	// exception
#include <fcntl.h>		// fcntl
#include <iostream>		// cout - cerr
#include <netdb.h>		// addrinfo
#include <sstream>		// stringstream
#include <string>		// string
#include <sys/poll.h>	// pollfd - poll
#include <sys/socket.h> // get / free addrinfo - socket - bind - listen - accept - send - recv
#include <sys/types.h>	// ( BSD )
#include <unistd.h>		// close
#include <vector>		// vector

namespace	irc {
	class	commands {
		private:
			std::string	cmd;
			std::string	arg;
		public:
			commands(std::string cmd, std::string arg);
			~commands();
			void	join(const irc::user &x, const std::string &message);
			void	part(const irc::user &x, const std::string &message);
			void	msg(const irc::user &x, const std::string &message);
			void	whois(const irc::user &x, const std::string &message);
			void	nick(const irc::user &x, const std::string &message);
			void	setUserName(const irc::user &x, const std::string &message);
			void	privmsg(const irc::user &x, const int &targetUser);
	};
}

#endif