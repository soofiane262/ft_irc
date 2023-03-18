/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acmaghou <acmaghou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 18:22:31 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/18 18:14:41 by acmaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

#include <cstdlib>

irc::server* irc::server::__serv		 = NULL;
std::string	 irc::server::__password	 = "";
std::string	 irc::server::__hostaddr	 = "";
std::string	 irc::server::__creationdate = "";

/* ctor ─────────────────────────────────────────────────────────────────────────────── */

irc::server::server( const int& ac, char** av ) {
	this->parse_args( ac, av );
	this->_buff = new char[ 513 ];
	// this->_shutdown_msg += "\n\n";
	// this->_shutdown_msg +=
	// 	"Thank you for usign our IRC server, it will be shutting down for maintenance.\n";
	// this->_shutdown_msg += "Until we chat again !";
	// this->_shutdown_msg += "\n";
	irc::server::__serv = this;
	std::signal( SIGINT, server::staticSigHandler );
	std::system( "ifconfig | grep 'inet ' | awk 'NR==2 {print $2}' > .log" );
	irc::utils::__ss << std::ifstream( ".log" ).rdbuf();
	std::system( "rm -f .log" );
	irc::server::__hostaddr =
		irc::utils::__ss.str().substr( 0, irc::utils::__ss.str().find( '\n' ) );
	irc::utils::__ss.str( std::string() );
	irc::utils::__ss.clear();
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

/* parse_args ─────────────────────────────────────────────────────────────────────── */

void irc::server::parse_args( const int& ac, char** av ) {
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
} // parse_args

/* signal_handler ─────────────────────────────────────────────────────────────────── */

void irc::server::staticSigHandler( int ) {
	irc::server::__serv->shutDownServer();
	exit( 0 );
} // signal_handler

/* accept_client ──────────────────────────────────────────────────────────────────── */

void irc::server::acceptClient( void ) {
	pollfd pfd;
	bzero( &pfd, sizeof( pfd ) );
	pfd.fd = accept( this->_sockets.front().fd, NULL, NULL );
	if ( pfd.fd == -1 ) return;
	pfd.events = POLLIN | POLLOUT;
	this->_sockets.push_back( pfd );
	this->_clients.insert( std::make_pair( pfd.fd, irc::client( this->_sockets.back() ) ) );
} // accept_client

/* diconnect_client ───────────────────────────────────────────────────────────────── */

void irc::server::disconClient( client_iterator& client_it_ ) {
	close( client_it_->second._pfd.fd );
	std::cout << "\033[2m"
			  << "User `" << client_it_->second._nickname << "` disconnected"
			  << "\033[22m\n";
	std::vector< pollfd >::iterator pfd_it = this->_sockets.begin();
	while ( pfd_it->fd != client_it_->second._pfd.fd ) pfd_it++;
	this->_sockets.erase( pfd_it );
	for ( channels_iterator channel_it = this->_channels.begin(); channel_it != _channels.end();
		  ++channel_it )
		channel_it->second._members.erase( client_it_->second._pfd.fd );
	this->_clients.erase( client_it_ );
	client_it_ = this->_clients.begin();
} // diconnect_client

/* send_message ───────────────────────────────────────────────────────────────────── */

void irc::server::sendMsg( client_iterator& client_it_ ) {
	if ( client_it_->second._msg_out.empty() ) return;
	int bytes_sent = send( client_it_->second._pfd.fd, client_it_->second._msg_out.c_str(),
						   client_it_->second._msg_out.length(), 0 );
	if ( bytes_sent == -1 ) THROW_EXCEPT( "Unable to send() data to client" );
	client_it_->second._msg_out.erase( 0, bytes_sent );
	if ( client_it_->second._pfd.revents & POLLHUP ) this->disconClient( client_it_ );
	// if ( client_it_->second._pfd.revents & POLLHUP ) std::cerr << "POLLHUP" << std::endl;
} // send_message

void irc::server::sendMsg( irc::client& client_ ) {
	if ( client_._msg_out.empty() ) return;
	int bytes_sent =
		send( client_._pfd.fd, client_._msg_out.c_str(), client_._msg_out.length(), 0 );
	if ( bytes_sent == -1 ) THROW_EXCEPT( "Unable to send() data to client" );
	client_._msg_out.erase( 0, bytes_sent );
} // send_message

/* receive_message ────────────────────────────────────────────────────────────────── */

void irc::server::recvMsg( irc::client& client_ ) {
	bzero( _buff, 513 );
	int bytes_rcvd = recv( client_._pfd.fd, _buff, 512, 0 );
	if ( bytes_rcvd == -1 ) {
		client_._msg_in.clear();
		THROW_EXCEPT( "Unable to recv() data from client" );
	}
	client_._msg_in.append( _buff, bytes_rcvd );
	if ( client_._msg_in.size() >= 2 &&
		 !client_._msg_in.compare( client_._msg_in.size() - 2, 2, CRLF ) )
		this->_commands[ client_ ];
} // receive_message

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

/* find_client_by_nickname ──────────────────────────────────────────────────────────── */

bool irc::server::findClientByNick( const std::string& nick_ ) {
	irc::server::client_iterator cl_it;
	for ( cl_it = this->_clients.begin();
		  cl_it != this->_clients.end() && cl_it->second._nickname.compare( nick_ ); cl_it++ ) {
			std::cout << "cl_it->second._nickname => "<< cl_it->second._nickname << std::endl;
			continue;
		  }
	return cl_it != this->_clients.end();
} // find_client_by_nickname
