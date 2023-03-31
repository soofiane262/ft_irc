/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hous <mel-hous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:22:31 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/31 17:27:25 by mel-hous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

/* static ──────────────────────────────────────────────────────────────────────────── */

irc::server* irc::server::__serv		 = NULL;
std::string	 irc::server::__operpass	 = "operpass";
std::string	 irc::server::__password	 = "";
std::string	 irc::server::__hostaddr	 = "";
std::string	 irc::server::__creationdate = "";

/* ctor ─────────────────────────────────────────────────────────────────────────────── */

irc::server::server( const int& ac, char** av ) {
	if ( ac != 3 ) ARGS_EXCEPT;
	int port = irc::utils::ft_stoi( av[ 1 ] );
	if ( port <= 0 || port > 65535 ) PORT_EXCEPT;
	this->_port				= port;
	irc::server::__password = av[ 2 ];
	if ( irc::server::__password.empty() ) PASS_EXCEPT;
	this->_buff			= new char[ 513 ];
	irc::server::__serv = this;
	std::signal( SIGINT, server::staticSigHandler );
	std::system( "ifconfig | grep 'inet ' | awk 'NR==2 {print $2}' > .log" );
	std::stringstream ss;
	ss << std::ifstream( ".log" ).rdbuf();
	std::system( "rm -f .log" );
	irc::server::__hostaddr = ss.str().substr( 0, ss.str().find( '\n' ) );
} // ctor

/* dtor ───────────────────────────────────────────────────────────────────────────── */

irc::server::~server( void ) {
	delete[] this->_buff;
	this->_buff = NULL;
} // dtor

/* accessors ────────────────────────────────────────────────────────────────────────── */

std::string irc::server::getClientsSize( void ) {
	std::stringstream ss;
	ss << this->_clients.size();
	return ss.str();
} // getClientsSize

irc::server::client_type	  &irc::server::getClients(){
	return(_clients);
};

std::string irc::server::getOpersSize( void ) {
	std::stringstream ss;
	int				  nb = 0;
	for ( irc::server::client_iterator it = this->_clients.begin(); it != this->_clients.end();
		  it++ ) {
		if ( it->second._mode & UMODE_OPERATOR ) nb++;
	}
	ss << nb;
	return ss.str();
} // getClientsSize

std::string irc::server::getChannelsSize( void ) {
	std::stringstream ss;
	ss << this->_channels.size();
	return ss.str();
} // getChannelsSize
