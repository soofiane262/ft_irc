/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/26 13:32:25 by mel-hous          #+#    #+#             */
/*   Updated: 2023/04/01 21:11:36 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

std::string irc::client::getModes( void ) {
	std::string modes = "+";
	if ( _mode & UMODE_AWAY ) modes += "a";
	if ( _mode & UMODE_WALLOPS ) modes += "w";
	if ( _mode & UMODE_INVISIBLE ) modes += "i";
	if ( _mode & UMODE_RESTRICTED ) modes += "r";
	if ( _mode & UMODE_OPERATOR ) modes += "o";
	return modes;
} // getModes

std::string irc::client::getChannels( void ) {
	std::string channels;
	for ( std::set< std::string >::iterator it = this->_channels_joined.begin();
		  it != this->_channels_joined.end(); it++ ) {
		channels += *it;
		channels += " ";
	}
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
	pollfd pfd;
	bzero( &pfd, sizeof( pfd ) );
	pfd.fd = accept( this->_sockets.front().fd, NULL, NULL );
	if ( pfd.fd == -1 ) return;
	pfd.events						   = POLLIN | POLLOUT;
	irc::server::poll_iterator poll_it = std::lower_bound(
		this->_sockets.begin(), this->_sockets.end(), pfd, irc::utils::pollfd_cmp );
	this->_sockets.insert( poll_it, pfd );
	this->_clients.insert( std::make_pair( pfd.fd, irc::client( pfd.fd ) ) );
} // accept_client

void irc::server::disconClient( client_iterator& client_it_, poll_iterator& poll_it_ ) {
	close( client_it_->second._fd );
	std::cout << "\033[2m"
			  << "User `" << client_it_->second._nickname << "` disconnected"
			  << "\033[22m\n";
	this->_sockets.erase( poll_it_ );
	for ( channel_iterator channel_it = this->_channels.begin(); channel_it != _channels.end();
		  ++channel_it )
		channel_it->second->_members.erase( &client_it_->second );
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
