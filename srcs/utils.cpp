/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 14:55:06 by sel-mars          #+#    #+#             */
/*   Updated: 2023/03/17 17:28:01 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

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

/* user_is_valid ────────────────────────────────────────────────────────────────────── */
/* BNF: user       =  1*( %x01-09 / %x0B-0C / %x0E-1F / %x21-3F / %x41-FF ) */

bool irc::utils::userIsValid( const std::string& user_ ) {
	if ( user_.empty() ) return false;
	std::string::const_iterator it = user_.begin();
	while ( it != user_.end() && !std::strchr( NULCRLFSPCL, *it ) ) ++it;
	if ( it != user_.end() ) return false;
	return true;
}
