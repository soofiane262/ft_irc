/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BOT.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:19:26 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/02 18:18:38 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../irc.hpp"

static std::string firstDayOfCurrentMonth( void ) {
	std::time_t now = std::time( nullptr );

	std::tm local_time = *std::localtime( &now );

	local_time.tm_mday = 1;
	local_time.tm_hour = 0;
	local_time.tm_min  = 0;
	local_time.tm_sec  = 0;

	std::stringstream ss;
	ss << std::put_time( &local_time, "%Y-%m-%d" );
	return ss.str();
}

static std::string ft_curl( const std::string& cmd_trail ) {
	const char* api_token = std::getenv( "API42_TOKEN" );
	if ( api_token == nullptr )
		throw std::runtime_error( "API42_TOKEN environment variable not set" );

	std::string cmd = "rm -f .log && touch .log && curl -sH \"Authorization: Bearer " +
					  std::string( api_token ) + "\" " + cmd_trail + " > .log";

	int				  status = std::system( cmd.c_str() );
	std::stringstream ss;
	ss << std::ifstream( ".log" ).rdbuf();
	std::system( "rm -f .log" );
	if ( status != 0 ) throw std::runtime_error( "curl command exited with non-zero status" );

	return ss.str();
}

void irc::commands::BOT( irc::client& client_ ) {
	if ( client_._message._params.size() < 2 || client_._message._params.front().empty() ) {
		client_._msg_out += ERR_NEEDMOREPARAMS( client_ );
		return;
	}

	const std::string& username = client_._message._params[ 1 ];
	std::string		   reply, curl_ret, cmd_trail = "\"https://api.intra.42.fr/v2/users/";

	try {
		if ( !client_._message._params.front().compare( "location" ) ) {
			cmd_trail +=
				client_._message._params[ 1 ] +
				"/locations?filter\\[end\\]=false\" | awk -F\'\"location\":\' \'{print $2}\' "
				"| awk -F\'\"\' \'{print $2}\'";
			curl_ret = ft_curl( cmd_trail );
			reply	 = "User " + username + " is currently " +
					( curl_ret.size() == 1 ? "unavailable" : ( "at " + curl_ret ) );
		} else if ( !client_._message._params.front().compare( "logtime" ) ) {
			cmd_trail +=
				client_._message._params[ 1 ] + "/locations_stats?begin_at=\\[" +
				firstDayOfCurrentMonth() +
				"\\]\" | grep -o '[0-9]\\{2\\}:[0-9]\\{2\\}' | awk -F: \'{sum += ($1 * 60) + $2} "
				"END {printf \"%d:%02d\\n\", sum/60, sum%60}\'";
			curl_ret = ft_curl( cmd_trail );
			reply	 = "User " + username + " logtime " + curl_ret;
		} else if ( !client_._message._params.front().compare( "coalition" ) ) {
			cmd_trail += client_._message._params[ 1 ] +
						 "/coalitions\" | awk -F\'\"name\":\' \'{print $2}\' "
						 "| awk -F\'\"\' \'{print $2}\'";
			curl_ret = ft_curl( cmd_trail );
			reply	 = "User " + username + " coalition " +
					( curl_ret.size() == 1 ? "is unavailable" : curl_ret );
		} else
			client_._msg_out += ERR_UNKNOWNCOMMAND( client_ );
	} catch ( const std::exception& e ) {
		std::cerr << "\033[31m" << e.what() << "\033[0m" << std::endl;
		reply = "Error: an internal error occured";
	}

	client_._msg_out += RPL_BOT( client_, reply );
}
