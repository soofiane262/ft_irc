/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:22:31 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/08 18:28:45 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

irc::server *irc::server::__serv = NULL;

/* ctor ───────────────────────────────────────────────────────────────────────────── */

irc::server::server( const int &ac, char **av ) {
	parse_args( ac, av );
	_buff		 = new char[ 513 ];
	_welcome_msg = "\033[2m";
	_welcome_msg += "Greetings and welcome to our Internet Relay Chat server !\n";
	_welcome_msg += "We hope you feel at home here.\n";
	_welcome_msg += "\033[22m\n";
	_shutdown_msg = "\n\n\033[2m";
	_shutdown_msg +=
		"Thank you for usign our IRC server, it will be shutting down for maintenance.\n";
	_shutdown_msg += "Until we chat again !";
	_shutdown_msg += "\033[22m\n";
} // ctor

/* dtor ───────────────────────────────────────────────────────────────────────────── */

irc::server::~server( void ) {
	delete[] _buff;
	_buff = NULL;
} // dtor

/* parse_args ─────────────────────────────────────────────────────────────────────── */

void irc::server::parse_args( const int &ac, char **av ) {
	if ( ac != 3 ) ARGS_EXCEPT;
	std::stringstream ss;
	ss << av[ 1 ];
	ss >> _port;
	ss.str( std::string() );
	ss.clear();
	ss << _port;
	std::string port_str( &av[ 1 ][ av[ 1 ][ 0 ] && av[ 1 ][ 0 ] == '+' ] );
	if ( ss.str().compare( port_str ) || !_port ) PORT_EXCEPT;
	_password = av[ 2 ];
} // parse_args

/* signal_handler ─────────────────────────────────────────────────────────────────── */

void irc::server::sigHandler( int sig ) {
	for ( int idx = static_cast< int >( _sockets.size() ) - 1; idx; --idx ) {
		if ( _sockets[ idx ].revents & POLLOUT ) {
			_clients[ idx - 1 ]._msg_out = _shutdown_msg;
			while ( !_clients[ idx - 1 ]._msg_out.empty() ) sendMsg( idx - 1 );
			close( _sockets[ idx ].fd );
		}
	}
	if ( close( _sockets.front().fd ) ) ERRNO_EXCEPT;
	std::cout << "\n\n\033[2m\033[4m"
			  << "IRC Server shut down successfully"
			  << "\033[22m\033[24m\n";
	exit( sig );
} // signal_handler

void irc::server::staticSigHandler( int sig ) { __serv->sigHandler( sig ); } // signal_handler

/* initialize_server ──────────────────────────────────────────────────────────────── */

void irc::server::initServer( void ) {
	addrinfo hints, *servinfo = NULL;
	bzero( &hints, sizeof( hints ) );
	hints.ai_family	  = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags	  = AI_PASSIVE;
	std::stringstream ss;
	ss << _port;
	int i = getaddrinfo( NULL, ss.str().c_str(), &hints, &servinfo );
	if ( i ) i == EAI_SYSTEM ? ERRNO_EXCEPT : ERRCODE_EXCEPT( i );
	pollfd mastersocket;
	bzero( &mastersocket, sizeof( mastersocket ) );
	mastersocket.fd = socket( servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol );
	mastersocket.events = POLLIN | POLLOUT;
	i					= 1;
	if ( mastersocket.fd == -1 ||
		 setsockopt( mastersocket.fd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof( i ) ) == -1 ||
		 fcntl( mastersocket.fd, F_SETFL, O_NONBLOCK ) == -1 ||
		 bind( mastersocket.fd, servinfo->ai_addr, servinfo->ai_addrlen ) == -1 ||
		 listen( mastersocket.fd, BACKLOG ) == -1 )
		ERRNO_EXCEPT;
	_sockets.push_back( mastersocket );
	freeaddrinfo( servinfo );
	std::cout << "\033[2m\033[4m"
			  << "IRC Server started successfully : port " << _port << "\033[22m\033[24m\n\n";
} // initialize_server

/* accept_client ──────────────────────────────────────────────────────────────────── */

void irc::server::acceptClient( void ) {
	pollfd pfd;
	bzero( &pfd, sizeof( pfd ) );
	pfd.fd = accept( _sockets.front().fd, NULL, NULL );
	if ( pfd.fd == -1 ) return;
	pfd.events = POLLIN | POLLOUT;
	_sockets.push_back( pfd );
	_clients.push_back( client() );
	std::cout << "\033[2m"
			  << "New client connected"
			  << "\033[22m\n";
	_clients.back()._msg_out = _welcome_msg;
} // accept_client

/* diconnect_client ───────────────────────────────────────────────────────────────── */

void irc::server::disconClient( const int client_pos ) {
	close( _sockets[ client_pos + 1 ].fd );
	std::cout << "\033[2m"
			  << "client `" << _clients[ client_pos ]._user._nickname << "` disconnected"
			  << "\033[22m\n";
	_sockets.erase( _sockets.begin() + client_pos + 1 );
	_clients.erase( _clients.begin() + client_pos );
} // diconnect_client

/* send_message ───────────────────────────────────────────────────────────────────── */

void irc::server::sendMsg( const int client_pos ) {
	if ( _clients[ client_pos ]._msg_out.empty() ) return;
	int bytes_sent = send( _sockets[ client_pos + 1 ].fd, _clients[ client_pos ]._msg_out.c_str(),
						   _clients[ client_pos ]._msg_out.length(), 0 );
	if ( bytes_sent == -1 ) THROW_EXCEPT( "Unable to send() data to client" );
	_clients[ client_pos ]._msg_out.erase( 0, bytes_sent );
} // send_message

/* receive_message ────────────────────────────────────────────────────────────────── */

void irc::server::recvMsg( const int client_pos ) {
	bzero( _buff, 513 );
	int bytes_rcvd = recv( _sockets[ client_pos + 1 ].fd, _buff, 512, 0 );
	if ( bytes_rcvd == -1 ) THROW_EXCEPT( "Unable to recv() data from client" );
	_clients[ client_pos ]._msg_in.append( _buff, bytes_rcvd );
	std::cout << _buff;
} // receive_message

/* run_server ─────────────────────────────────────────────────────────────────────── */

void irc::server::runServer( void ) {
	__serv = this;
	std::signal( SIGINT, server::staticSigHandler );
	while ( 1 ) {
		if ( poll( &_sockets.front(), _sockets.size(), -1 ) == -1 ) ERRNO_EXCEPT;
		if ( _sockets.front().revents == POLLIN ) acceptClient();
		for ( int idx = 0; idx < static_cast< int >( _clients.size() ); ++idx ) {
			if ( _sockets[ idx + 1 ].revents & ( POLLERR | POLLHUP | POLLNVAL ) ) {
				disconClient( idx-- );
				continue;
			}
			if ( _sockets[ idx + 1 ].revents & POLLIN ) recvMsg( idx );
			if ( _sockets[ idx + 1 ].revents & POLLOUT ) sendMsg( idx );
		}
	}
} // run_server
