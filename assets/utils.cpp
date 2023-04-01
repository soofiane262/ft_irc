/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 14:55:06 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/01 20:49:14 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

std::string irc::utils::toLower( std::string& str_ ) {
	std::transform( str_.begin(), str_.end(), str_.begin(), ::tolower );
	return str_;
} // toLower

int irc::utils::ft_stoi( const std::string& str_ ) {
	int				  ret;
	std::stringstream ss;
	ss << str_;
	ss >> ret;
	ss.str( std::string() );
	ss.clear();
	ss << ret;
	std::string _str( &str_[ str_[ 0 ] && str_[ 0 ] == '+' ] );
	if ( ss.str().compare( _str ) ) ret = -1;
	return ret;
} // string_to_int

unsigned char irc::utils::intToMode( const int& mode_ ) {
	unsigned char ret = 0;
	if ( mode_ & UMODE_AWAY ) ret |= UMODE_AWAY;
	if ( mode_ & UMODE_WALLOPS ) ret |= UMODE_WALLOPS;
	if ( mode_ & UMODE_INVISIBLE ) ret |= UMODE_INVISIBLE;
	if ( mode_ & UMODE_RESTRICTED ) ret |= UMODE_RESTRICTED;
	if ( mode_ & UMODE_OPERATOR ) ret |= UMODE_OPERATOR;
	if ( !ret ) ret |= UMODE_WALLOPS;
	return ret;
} // parse_mode

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

bool irc::utils::pollfd_cmp( const pollfd& pfd_a_, const pollfd& pfd_b_ ) {
	return pfd_a_.fd < pfd_b_.fd;
} // pollfd_cmp

std::vector< std::string > irc::utils::split( const std::string& str_, const char& sep_ ) {
	std::vector< std::string >	ret;
	std::string::const_iterator it = str_.begin();
	std::string::const_iterator it2;

	while ( it != str_.end() ) {
		it2 = std::find( it, str_.end(), sep_ );
		ret.push_back( std::string( it, it2 ) );
		it = it2;
		if ( it != str_.end() ) ++it;
	}
	return ret;
} // split
