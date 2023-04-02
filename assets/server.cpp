/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:22:31 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/02 16:28:27 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

irc::server* irc::server::__serv		 = NULL;
bool		 irc::server::__shutdown	 = false;
bool		 irc::server::__restart		 = false;
std::string	 irc::server::__operpass	 = "operpass";
std::string	 irc::server::__password	 = "";
std::string	 irc::server::__hostaddr	 = "";
std::string	 irc::server::__creationdate = "";

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

irc::server::~server( void ) {
	delete[] this->_buff;
	this->_buff = NULL;
} // dtor

void irc::server::staticSigHandler( int ) { irc::server::__shutdown = true; } // signal_handler

/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */
/*                                   Server statistics                                  */
/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

std::string irc::server::getClientsSize( void ) {
	std::stringstream ss;
	ss << this->_clients.size();
	return ss.str();
} // getClientsSize

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

/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */
/*                        Server initialisation, run and shutdown                       */
/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

void irc::server::initServer( void ) {
	addrinfo hints, *servinfo = NULL, *ptr = NULL;
	bzero( &hints, sizeof( hints ) );
	hints.ai_family	  = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags	  = AI_PASSIVE;
	std::stringstream ss;
	ss << this->_port;
	int i = getaddrinfo( NULL, ss.str().c_str(), &hints, &servinfo );
	if ( i ) i == EAI_SYSTEM ? ERRNO_EXCEPT : ERRCODE_EXCEPT( i );
	pollfd mastersocket;
	bzero( &mastersocket, sizeof( mastersocket ) );
	for ( ptr = servinfo; ptr; ptr = ptr->ai_next )
		if ( ( mastersocket.fd = socket( servinfo->ai_family, servinfo->ai_socktype,
										 servinfo->ai_protocol ) ) == -1 )
			continue;
	mastersocket.events = POLLIN | POLLOUT;
	i					= 1;
	if ( mastersocket.fd == -1 ||
		 setsockopt( mastersocket.fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof( i ) ) == -1 ||
		 fcntl( mastersocket.fd, F_SETFL, O_NONBLOCK ) == -1 ||
		 bind( mastersocket.fd, servinfo->ai_addr, servinfo->ai_addrlen ) == -1 ||
		 listen( mastersocket.fd, BACKLOG ) == -1 )
		ERRNO_EXCEPT;
	this->_sockets.push_back( mastersocket );
	freeaddrinfo( servinfo );
	std::time_t rawtime = std::time( NULL );
	( irc::server::__creationdate = std::asctime( std::localtime( &rawtime ) ) )
		.erase( this->__creationdate.end() - 1 );
} // initialize_server

void irc::server::runServer( void ) {
	client_iterator client_it;
	poll_iterator	poll_it;
run:
	std::cout << "\033[2m\033[4m"
			  << "IRC Server started successfully ~ " << this->__hostaddr << ':' << this->_port
			  << "\033[22m\033[24m\n\n";
	while ( 1 ) {
		if ( poll( &this->_sockets.front(), this->_sockets.size(), -1 ) == -1 ) ERRNO_EXCEPT;
		if ( this->_sockets.front().revents == POLLIN ) acceptClient();
		for ( poll_it = this->_sockets.begin() + 1, client_it = this->_clients.begin();
			  client_it != this->_clients.end();
			  client_it != this->_clients.end() ? ++client_it : client_it,
			  poll_it != this->_sockets.end() ? ++poll_it : poll_it ) {
			if ( poll_it->revents & ( POLLERR | POLLHUP | POLLNVAL ) ) {
				disconClient( client_it, poll_it );
				continue;
			}
			try {
				if ( poll_it->revents & POLLIN ) recvMsg( client_it->second );
				if ( poll_it->revents & POLLOUT && !client_it->second._msg_out.empty() )
					sendMsg( client_it->second );
				if ( this->__shutdown || this->__restart ) goto shutdown;
			} catch ( std::exception& e ) { std::cerr << "\033[1;31m" << e.what() << "\033[0m\n"; }
			if ( client_it->second._quit ) disconClient( client_it, poll_it );
		}
	}
shutdown:
	this->shutdownServer();
	if ( this->__shutdown ) return;
	this->initServer();
	this->__restart = false;
	goto run;
} // run_server

void irc::server::shutdownServer( void ) {
	client_iterator client_it;
	for ( client_it = this->_clients.begin(); client_it != this->_clients.end(); ++client_it ) {
		try {
			client_it->second._msg_out = RPL_CLOSINGLINK( client_it->second );
			sendMsg( client_it->second );
		} catch ( ... ) {}
		close( client_it->second._fd );
	}
	if ( close( this->_sockets.begin()->fd ) ) ERRNO_EXCEPT;
	this->_channels.clear();
	this->_clients.clear();
	this->_sockets.clear();
	std::cout << "\n\n\033[2m\033[4m"
			  << "IRC Server shut down successfully"
			  << "\033[22m\033[24m\n";
	std::system( "leaks ircserv" );
} // signal_handler
