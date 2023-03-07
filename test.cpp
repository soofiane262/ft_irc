#include <arpa/inet.h>
#include <clocale>
#include <cmath>
#include <cstring>
#include <exception>
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <strings.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>

#define THROW_EXCEPT( str ) throw std::runtime_error( str );
#define ARGS_EXCEPT			THROW_EXCEPT( "Usage: ./ircserv <port> <password>" );
#define ERRNO_EXCEPT		THROW_EXCEPT( std::strerror( errno ) );
#define BACKLOG				10

class ft_irc {
  private:
	class client {
	  private:
		pollfd		_pfd;
		std::string _name;

	  public:
		client( void ) {}
		client( const pollfd &pfd_, const std::string &name_ ) : _pfd( pfd_ ), _name( name_ ) {}
	};

  public:
	std::string			  in_port;
	std::string			  welcome_msg;
	std::vector< client > _clients;
	std::vector< pollfd > _pfds;
	ft_irc( void ) {
		welcome_msg += "\n";
		welcome_msg += "\033[2m";
		welcome_msg += "Greetings and welcome to our internet relay chat server !\n";
		welcome_msg += "\n";
		welcome_msg +=
			"We're delighted to have you join us and look forward to getting to know you better.\n";
		welcome_msg += "and we're sure that you'll fit right in.\n";
		welcome_msg += "\033[22m";
		welcome_msg += "\n";
	}
	void parse_args( int ac, char **av ) {
		int port_nb;
		if ( ac != 3 ) ARGS_EXCEPT;
		std::stringstream ss;
		ss << av[ 1 ];
		ss >> port_nb;
		ss.str( std::string() );
		ss.clear();
		ss << port_nb;
		if ( ss.str().compare( std::string( &av[ 1 ][ av[ 1 ][ 0 ] && av[ 1 ][ 0 ] == '+' ] ) ) ||
			 port_nb < 1 || port_nb > 65535 )
			THROW_EXCEPT( "Port should be an integer in range [1, 65535]" );
		in_port = ss.str();
		ss.str( std::string() );
		ss.clear();
	}
	void acceptClient( void ) {
		pollfd pfd;
		bzero( &pfd, sizeof( pfd ) );
		pfd.fd = accept( _pfds.front().fd, NULL, NULL );
		if ( pfd.fd == -1 ) ERRNO_EXCEPT;
		pfd.events = POLLIN | POLLOUT;
		_pfds.push_back( pfd );
		_clients.push_back( client( pfd, "" ) );
		std::cout << "\033[2m"
				  << "New client connected"
				  << "\033[22m\n";
		_pfds.back().revents = POLLOUT;
		sendMsg( _pfds.back(), welcome_msg );
	}
	void sendMsg( pollfd &client, std::string &msg ) {
		int bytes_sent = ( client.revents & POLLOUT ? 0 : -1 ), bytes_to_send = msg.length();
		while ( bytes_sent != -1 && bytes_sent != bytes_to_send )
			bytes_sent = send( client.fd, msg.c_str() + bytes_sent, bytes_to_send - bytes_sent, 0 );
		if ( bytes_sent == -1 ) THROW_EXCEPT( "Unable to send() data to client" );
	}
	void recvMsg( pollfd &client ) {
		char		buff[ 10 ];
		std::string msg( 10, '\0' );
		int			bytes_recvd = 0;
		while ( bytes_recvd != -1 && msg.back() != '\n' ) {
			bzero( buff, 10 );
			bytes_recvd = recv( client.fd, buff, 9, 0 );
			msg.append( buff );
		}
		if ( bytes_recvd == -1 ) THROW_EXCEPT( "Unable to recv() data from client" );
		sendMsg( client, msg );
	}
};

int main( int ac, char **av ) {
	try {
		ft_irc irc;
		irc.parse_args( ac, av );
		int		 one = 1;
		addrinfo hints;
		bzero( &hints, sizeof( hints ) );
		hints.ai_family	  = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags	  = AI_PASSIVE;
		addrinfo *servinfo, *ptr;
		int		  status = getaddrinfo( NULL, irc.in_port.c_str(), &hints, &servinfo );
		if ( status ) ERRNO_EXCEPT;
		pollfd mastersock;
		bzero( &mastersock, sizeof( mastersock ) );
		mastersock.fd = socket( servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol );
		mastersock.events = POLLIN | POLLOUT;
		if ( mastersock.fd == -1 ||
			 setsockopt( mastersock.fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof( one ) ) ||
			 fcntl( mastersock.fd, F_SETFL, O_NONBLOCK ) ||
			 bind( mastersock.fd, servinfo->ai_addr, servinfo->ai_addrlen ) ||
			 listen( mastersock.fd, BACKLOG ) )
			ERRNO_EXCEPT;
		irc._pfds.push_back( mastersock );
		std::cout << "\033[2m\033[4m"
				  << "IRC Server successfully started on port " << irc.in_port
				  << "\033[22m\033[24m\n\n";
		while ( 1 ) {
			if ( poll( &irc._pfds.front(), irc._pfds.size(), -1 ) == -1 ) ERRNO_EXCEPT;
			if ( irc._pfds.front().revents == POLLIN ) irc.acceptClient();
			for ( std::vector< pollfd >::iterator poll_it = irc._pfds.begin() + 1;
				  poll_it != irc._pfds.end(); ++poll_it ) {
				if ( poll_it->revents & POLLIN ) irc.recvMsg( *poll_it );
			}
		}
		if ( close( mastersock.fd ) ) ERRNO_EXCEPT;
		freeaddrinfo( servinfo );
		return 0;
	} catch ( std::exception &e ) {
		std::cerr << "\033[1;31m" << e.what() << '\n';
		return 1;
	}
}
