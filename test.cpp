#include <arpa/inet.h>
#include <clocale>
#include <cmath>
#include <cstring>
#include <exception>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <strings.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define THROW_EXCEPT( str ) throw std::runtime_error( str );
#define ARGS_EXCEPT			THROW_EXCEPT( "usage: ./ircserv <port> <password>" );
#define ERRNO_EXCEPT		THROW_EXCEPT( std::strerror( errno ) );
#define BACKLOG				10

typedef struct s_ftirc {
	std::string in_port;
	void		parse_args( int ac, char **av ) {
		   int port_nb;
		   if ( ac != 3 ) ARGS_EXCEPT;
		   std::stringstream ss;
		   ss << av[ 1 ];
		   ss >> port_nb;
		   ss.str( std::string() );
		   ss.clear();
		   ss << port_nb;
		   std::cout << port_nb << '\n';
		   if ( ss.str().compare( std::string( &av[ 1 ][ av[ 1 ][ 0 ] && av[ 1 ][ 0 ] == '+' ] ) ) ||
				port_nb < 1 || port_nb > 65535 )
			   THROW_EXCEPT( "port number should be an integer in range [1, 65535]" );
		   in_port = ss.str();
		   ss.str( std::string() );
		   ss.clear();
	}
} t_ftirc;

int main( int ac, char **av ) {
	try {
		t_ftirc irc;
		irc.parse_args( ac, av );
		int				one = 1;
		struct addrinfo hints;
		bzero( &hints, sizeof( hints ) );
		hints.ai_family	  = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_flags	  = AI_PASSIVE;
		struct addrinfo *servinfo, *ptr;
		int				 status = getaddrinfo( NULL, irc.in_port.c_str(), &hints, &servinfo );
		if ( status ) ERRNO_EXCEPT;
		int mastersock_fd =
			socket( servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol );
		if ( mastersock_fd == -1 ||
			 setsockopt( mastersock_fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof( one ) ) ||
			 bind( mastersock_fd, servinfo->ai_addr, servinfo->ai_addrlen ) ||
			 listen( mastersock_fd, BACKLOG ) )
			ERRNO_EXCEPT;
		struct sockaddr_storage client;
		socklen_t				addr_len = sizeof( client );
		int						clientsock_fd =
			accept( mastersock_fd, reinterpret_cast< sockaddr * >( &client ), &addr_len );
		if ( clientsock_fd == -1 ) ERRNO_EXCEPT;
		close( clientsock_fd );
		if ( close( mastersock_fd ) ) ERRNO_EXCEPT;
		freeaddrinfo( servinfo );
		return 0;
	} catch ( std::exception &e ) {
		std::cerr << "\033[1;31m" << e.what() << '\n';
		return 1;
	}
}
