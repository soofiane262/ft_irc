/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 14:55:06 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/28 17:31:53 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

/* ──────────────────────────────────────────────────────────────────────────────────── */
/* ──────────────────────────────────────────────────────────────────────────────────── */
// tmp - print message infos
std::ostream& irc::operator<<( std::ostream& o_, irc::message& message_ ) {
	o_ << "\n\033[2mmessage infos:\n";
	o_ << "\tcommand\t\t: ";
	if ( message_._command.empty() ) o_ << "<empty>";
	else
		o_ << message_._command << "\n";
	o_ << "\tparams\t\t: ";
	if ( message_._params.empty() ) o_ << "<empty>";
	else {
		o_ << "size " << message_._params.size();
		for ( std::size_t idx = 0; idx < message_._params.size(); ++idx )
			o_ << "\n\t\t" << idx << "\t--> `" << message_._params[ idx ] << "`";
	}
	o_ << "\033[22m\n";
	return o_;
}

// tmp - print client infos
std::ostream& irc::operator<<( std::ostream& o_, irc::client& client_ ) {
	o_ << "\n\033[2mclient infos:\n";
	o_ << "\tfd\t\t: " << client_._fd << "\n";
	o_ << "\tnickname\t: " << client_._nickname << "\n";
	o_ << "\tusername\t: " << client_._username << "\n";
	o_ << "\trealname\t: " << client_._realname << "\n";
	o_ << "\tmode\t\t: " << static_cast< int >( client_._mode ) << "\n";
	// o_ << "\t\033[2mchannels\t: ";
	// if ( client_._channels.empty() ) o_ << "<empty>";
	// else {
	// o_ << "size " << client_._channels.size();
	// for ( std::size_t idx = 0; idx < client_._channels.size(); ++idx )
	// o_ << "\n" << idx << "\t--> `\033[22m" << client_._channels[ idx ] << "\033[2m`";
	// }
	o_ << "\033[22m\n";
	return o_;
}
// tmp
/* ──────────────────────────────────────────────────────────────────────────────────── */
/* ──────────────────────────────────────────────────────────────────────────────────── */

/* nick_is_valid ────────────────────────────────────────────────────────────────────── */
/* BNF: nickname   =  ( letter / special ) *8( letter / digit / special / "-" ) */
bool irc::utils::nickIsValid( const std::string& nick_ ) {
	if ( nick_.empty() || nick_.length() > 9 ) return false;
	if ( !std::isalpha( nick_[ 0 ] ) && !std::strchr( SPECIAL_CHARACTERS, nick_[ 0 ] ) )
		return false;
	std::string::const_iterator it = nick_.begin() + 1;
	while ( it != nick_.end() &&
			( std::isalnum( *it ) || std::strchr( SPECIAL_CHARACTERS, *it ) || *it == '-' ) )
		++it;
	if ( it != nick_.end() ) return false;
	return true;
} // nickIsValid

bool irc::utils::pollfd_cmp( const pollfd& pfd_a_, const pollfd& pfd_b_ ) {
	return pfd_a_.fd < pfd_b_.fd;
}

/* channel_name_is_valid ────────────────────────────────────────────────────────────── */
/* BNF: chan_name = "#" *50(%x01-07 / %x08-09 / %x0B-0C / %x0E-1F / %x21-2B / %x2D-39 / %x3B-FF) */
bool irc::utils::channelNameIsValid( const std::string& channel_name_ ) {
	if ( channel_name_.length() < 2 || channel_name_.length() > 51 || channel_name_[ 0 ] != '#' )
		return false;
	std::string::const_iterator it = channel_name_.begin() + 1;
	while ( it != channel_name_.end() && !std::strchr( NULCRLFSPCL, *it ) && *it != BELL &&
			*it != COLON )
		++it;
	if ( it != channel_name_.end() ) return false;
	return true;
} // channelNameIsValid

/* parse_mode ───────────────────────────────────────────────────────────────────────── */

unsigned char irc::utils::intToMode( const int& mode_ ) {
	unsigned char ret = 0;
	if ( mode_ & UMODE_AWAY ) ret |= UMODE_AWAY;
	if ( mode_ & UMODE_WALLOPS ) ret |= UMODE_WALLOPS;
	if ( mode_ & UMODE_INVISIBLE ) ret |= UMODE_INVISIBLE;
	if ( mode_ & UMODE_RESTRICTED ) ret |= UMODE_RESTRICTED;
	if ( mode_ & UMODE_OPERATOR ) ret |= UMODE_OPERATOR;
	if ( mode_ & UMODE_RECEIPT ) ret |= UMODE_RECEIPT;
	return ret;
}

void msg_out_creator( irc::client& client_, std::string error_reply ) {
	client_._msg_out += error_reply;
}
