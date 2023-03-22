/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_ops.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acmaghou <acmaghou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 17:01:48 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/21 18:06:15 by acmaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

/* find_client_by_nickname ──────────────────────────────────────────────────────────── */
bool irc::server::findClientByNick( const std::string& nick_ ) {
	irc::server::client_iterator cl_it;
	for ( cl_it = this->_clients.begin();
		  cl_it != this->_clients.end() && cl_it->second._nickname.compare( nick_ ); cl_it++ )
		continue;
	return cl_it != this->_clients.end();
} // find_client_by_nickname

irc::client* irc::server::findClient( const std::string& nick_ ) {
	irc::server::client_iterator cl_it;
	for ( cl_it = this->_clients.begin();
		  cl_it != this->_clients.end() && cl_it->second._nickname.compare( nick_ ); cl_it++ )
		continue;
	return cl_it != this->_clients.end() ? &cl_it->second : NULL;
} // find_client_by_nickname


/* accept_client ──────────────────────────────────────────────────────────────────── */

void irc::server::acceptClient( void ) {
	pollfd pfd;
	bzero( &pfd, sizeof( pfd ) );
	pfd.fd = accept( this->_sockets.front().fd, NULL, NULL );
	if ( pfd.fd == -1 ) return;
	pfd.events                           = POLLIN | POLLOUT;
	irc::server::poll_iterator poll_it = std::lower_bound(
		this->_sockets.begin(), this->_sockets.end(), pfd, irc::utils::pollfd_cmp );
	this->_sockets.insert( poll_it, pfd );
	this->_clients.insert( std::make_pair( pfd.fd, irc::client( pfd.fd ) ) );
} // accept_client

/* diconnect_client ───────────────────────────────────────────────────────────────── */

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
