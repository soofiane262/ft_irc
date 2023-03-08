/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 14:41:31 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/08 18:14:25 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

int main( int ac, char **av ) {
	try {
		irc::server ircserv( ac, av );
		ircserv.initServer();
		ircserv.runServer();
		return 0;
	} catch ( std::exception &e ) {
		std::cerr << "\033[1;31m" << e.what() << '\n';
		return 1;
	}
}
