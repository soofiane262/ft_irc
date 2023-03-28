/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:22:31 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/28 15:45:45 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

/* static ──────────────────────────────────────────────────────────────────────────── */

irc::server* irc::server::__serv		 = NULL;
std::string	 irc::server::__password	 = "";
std::string	 irc::server::__hostaddr	 = "";
std::string	 irc::server::__creationdate = "";

/* ctor ─────────────────────────────────────────────────────────────────────────────── */

irc::server::server( const int& ac, char** av ) {
	if ( ac != 3 ) ARGS_EXCEPT;
	std::stringstream ss;
	ss << av[ 1 ];
	ss >> this->_port;
	ss.str( std::string() );
	ss.clear();
	ss << this->_port;
	std::string port_str( &av[ 1 ][ av[ 1 ][ 0 ] && av[ 1 ][ 0 ] == '+' ] );
	if ( ss.str().compare( port_str ) || !this->_port ) PORT_EXCEPT;
	irc::server::__password = av[ 2 ];
	if ( irc::server::__password.empty() ) PASS_EXCEPT;
	this->_buff			= new char[ 513 ];
	irc::server::__serv = this;
	std::signal( SIGINT, server::staticSigHandler );
	std::system( "ifconfig | grep 'inet ' | awk 'NR==2 {print $2}' > .log" );
	ss.str( std::string() );
	ss.clear();
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

std::string irc::server::getChannelsSize( void ) {
	std::stringstream ss;
	ss << this->_channels.size();
	return ss.str();
} // getChannelsSize
