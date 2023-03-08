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
	char *_buff;
	class client {
	  public:
		std::string _nickname;
		std::string _msg_in;
		std::string _msg_out;
		client( void ) {}
	};
	class channel {
	  public:
		std::string _name;
		channel( void ) {}
	};

  public:
	std::string			   in_port;
	std::string			   password;
	std::string			   welcome_msg;
	std::vector< channel > _channels;
	std::vector< client >  _clients;
	std::vector< pollfd >  _pfds;
	ft_irc( void ) {
		_buff = new char[ 512 ];
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
	~ft_irc( void ) {
		delete[] _buff;
		_buff = NULL;
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
		password = av[ 2 ];
	}
	void acceptClient( void ) {
		pollfd pfd;
		bzero( &pfd, sizeof( pfd ) );
		pfd.fd = accept( _pfds.front().fd, NULL, NULL );
		if ( pfd.fd == -1 ) ERRNO_EXCEPT;
		pfd.events = POLLIN | POLLOUT;
		_pfds.push_back( pfd );
		_clients.push_back( client() );
		std::cout << "\033[2m"
				  << "New client connected"
				  << "\033[22m\n";
		_clients.back()._msg_out = welcome_msg;
	}
	void disconClient( const int client_pos ) {
		close( _pfds[ client_pos + 1 ].fd );
		_pfds.erase( _pfds.begin() + client_pos + 1 );
		_clients.erase( _clients.begin() + client_pos );
	}
	void sendMsg( const int client_pos ) {
		if ( _clients[ client_pos ]._msg_out.empty() ) return;
		int bytes_sent = send( _pfds[ client_pos + 1 ].fd, _clients[ client_pos ]._msg_out.c_str(),
							   _clients[ client_pos ]._msg_out.length(), 0 );
		if ( bytes_sent == -1 ) THROW_EXCEPT( "Unable to send() data to client" );
		_clients[ client_pos ]._msg_out.erase( 0, bytes_sent );
	}
	void recvMsg( const int client_pos ) {
		bzero( _buff, 1000 );
		int bytes_rcvd = recv( _pfds[ client_pos + 1 ].fd, _buff, 999, 0 );
		if ( bytes_rcvd == -1 ) THROW_EXCEPT( "Unable to recv() data from client" );
		_clients[ client_pos ]._msg_in.append( _buff, bytes_rcvd );
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
			for ( int idx = 0; idx < irc._pfds.size(); ++idx ) {
				if ( irc._pfds[ idx + 1 ].revents & ( POLLERR | POLLHUP | POLLNVAL ) ) {
					irc.disconClient( idx );
					continue;
				}
				if ( irc._pfds[ idx + 1 ].revents & POLLIN ) irc.recvMsg( idx );
				if ( irc._pfds[ idx + 1 ].revents & POLLOUT ) irc.sendMsg( idx );
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
