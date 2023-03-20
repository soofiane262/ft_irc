/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_ops.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:01:00 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/20 17:01:32 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

/* signal_handler ─────────────────────────────────────────────────────────────────── */

void irc::server::staticSigHandler( int ) {
	irc::server::__serv->shutDownServer();
	exit( 0 );
} // signal_handler

/* initialize_server ──────────────────────────────────────────────────────────────── */

void irc::server::initServer( void ) {
	addrinfo hints, *servinfo = NULL, *ptr = NULL;
	bzero( &hints, sizeof( hints ) );
	hints.ai_family	  = AF_UNSPEC;
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
	( irc::server::__creationdate = std::asctime( localtime( &rawtime ) ) )
		.erase( this->__creationdate.end() - 1 );
} // initialize_server

/* run_server ─────────────────────────────────────────────────────────────────────── */

void irc::server::runServer( void ) {
	client_iterator client_it;
	std::cout << "\033[2m\033[4m"
			  << "IRC Server started successfully ~ " << this->__hostaddr << ':' << this->_port
			  << "\033[22m\033[24m\n\n";
	while ( 1 ) {
		if ( poll( &this->_sockets.front(), this->_sockets.size(), -1 ) == -1 ) ERRNO_EXCEPT;
		if ( this->_sockets.front().revents == POLLIN ) acceptClient();
		for ( client_it = this->_clients.begin(); client_it != this->_clients.end();
			  client_it != this->_clients.end() ? ++client_it : client_it ) {
			if ( client_it->second._pfd.revents & ( POLLERR | POLLHUP | POLLNVAL ) ) {
				disconClient( client_it );
				continue;
			}
			try {
				if ( client_it->second._pfd.revents & POLLIN ) recvMsg( client_it->second );
				if ( client_it->second._pfd.revents & POLLOUT ) sendMsg( client_it );
			} catch ( std::exception& e ) { std::cerr << "\033[1;31m" << e.what() << "\033[0m\n"; }
		}
	}
} // run_server

/* shut_down_server ─────────────────────────────────────────────────────────────────── */

void irc::server::shutDownServer( void ) {
	client_iterator client_it;
	for ( client_it = this->_clients.begin(); client_it != this->_clients.end(); ++client_it ) {
		try {
			while ( !client_it->second._msg_out.empty() ) sendMsg( client_it->second );
		} catch ( ... ) {}
		close( client_it->second._pfd.fd );
	}
	if ( close( this->_sockets.begin()->fd ) ) ERRNO_EXCEPT;
	std::cout << "\n\n\033[2m\033[4m"
			  << "IRC Server shut down successfully"
			  << "\033[22m\033[24m\n";
} // signal_handler
