/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 13:32:25 by mel-hous          #+#    #+#             */
/*   Updated: 2023/04/02 17:06:11 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

#include <netinet/in.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>

std::string irc::client::getModes( void ) {
	std::string modes = "+";
	if ( _mode & UMODE_AWAY ) modes += "a";
	if ( _mode & UMODE_WALLOPS ) modes += "w";
	if ( _mode & UMODE_INVISIBLE ) modes += "i";
	if ( _mode & UMODE_RESTRICTED ) modes += "r";
	if ( _mode & UMODE_OPERATOR ) modes += "o";
	return modes;
} // getModes

std::string irc::client::getChannels( char c_ ) {
	std::string channels;
	for ( std::set< std::string >::iterator it = this->_channels_joined.begin();
		  it != this->_channels_joined.end(); it++ ) {
		channels += *it;
		channels += c_;
	}
	if ( !channels.empty() ) channels.erase( channels.end() - 1 );
	return channels;
} // getChannels

std::string irc::client::getSignOnTime( void ) {
	std::stringstream ss;
	ss << this->_sign;
	return ss.str();
} // getSignOnTime

std::string irc::client::getIdleTime( void ) {
	std::stringstream ss;
	ss << std::difftime( time( NULL ), this->_idle );
	return ss.str();
} // getIdleTime

bool irc::client::isInvited( std::string& channel_name_ ) {
	return this->_channels_invited.find( channel_name_ ) != this->_channels_invited.end();
} // isInvited

bool irc::client::isInChannel( std::string& channel_name_ ) {
	return this->_channels_joined.find( channel_name_ ) != this->_channels_joined.end();
} // isInChannel

void irc::client::inviteChannel( std::string& channel_name_ ) {
	this->_channels_invited.insert( channel_name_ );
} // inviteChannel

void irc::client::joinChannel( std::string& channel_name_ ) {
	this->_channels_joined.insert( channel_name_ );
	this->_channels_invited.erase( channel_name_ );
} // joinChannel

void irc::client::partChannel( std::string& channel_name_ ) {
	this->_channels_joined.erase( channel_name_ );
} // partChannel

/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */
/*                                     Server client                                    */
/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

void irc::server::acceptClient( void ) {
	if ( this->_clients.size() >= MAX_FD ) return;
	pollfd pfd;
	bzero( &pfd, sizeof( pfd ) );
	sockaddr_in client_addr;
	socklen_t	client_addr_len = sizeof( client_addr );
	pfd.fd = accept( this->_sockets.front().fd, (sockaddr*)( &client_addr ), &client_addr_len );
	if ( pfd.fd == -1 ) return;
	pfd.events						   = POLLIN | POLLOUT;
	irc::server::poll_iterator poll_it = std::lower_bound(
		this->_sockets.begin(), this->_sockets.end(), pfd, irc::utils::pollfd_cmp );
	this->_sockets.insert( poll_it, pfd );
	std::string client_hostaddr = inet_ntoa( client_addr.sin_addr );
	if ( client_hostaddr == "127.0.0.1" ) client_hostaddr = irc::server::__hostaddr;
	this->_clients.insert( std::make_pair( pfd.fd, irc::client( pfd.fd ) ) )
		.first->second._hostaddr = client_hostaddr;
} // accept_client

void irc::server::disconClient( client_iterator& client_it_, poll_iterator& poll_it_ ) {
	client_it_->second._msg_in =
		"PART " + client_it_->second.getChannels( ',' ) + SPACE + client_it_->second._away_msg;
	this->execCommand( client_it_->second );
	irc::client* target;
	for ( irc::client::stringset_iterator it = client_it_->second._clients_connnected.begin();
		  it != client_it_->second._clients_connnected.end(); it++ ) {
		target = this->findClient( *it );
		if ( target ) target->_msg_out += RPL_QUIT( client_it_->second );
	}
	close( client_it_->second._fd );
	std::cout << "\033[2m"
			  << "User `" << client_it_->second._nickname << "` disconnected"
			  << "\033[22m\n";
	this->_sockets.erase( poll_it_ );
	this->_clients.erase( client_it_ );
	poll_it_   = this->_sockets.begin();
	client_it_ = this->_clients.begin();
} // diconnect_client

bool irc::server::isClient( const std::string& nick_ ) {
	irc::server::client_iterator cl_it;
	for ( cl_it = this->_clients.begin();
		  cl_it != this->_clients.end() && cl_it->second._nickname.compare( nick_ ); cl_it++ )
		continue;
	return cl_it != this->_clients.end();
} // isClient

irc::client* irc::server::findClient( const std::string& nick_ ) {
	irc::server::client_iterator cl_it;
	for ( cl_it = this->_clients.begin();
		  cl_it != this->_clients.end() && cl_it->second._nickname.compare( nick_ ); cl_it++ )
		continue;
	return cl_it != this->_clients.end() ? &cl_it->second : NULL;
} // findClient

irc::server::client_type& irc::server::getClients( void ) { return this->_clients; }; // getClients
