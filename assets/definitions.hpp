/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   definitions.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:28:37 by sel-mars          #+#    #+#             */
/*   Updated: 2023/04/02 23:50:28 by sel-mars         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */
/*                                        General                                       */
/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/* Constants ────────────────────────────────────────────────────────────────────────── */

#define MAX_FD			   1024
#define MAX_USERS		   "1024"
#define BACKLOG			   10
#define CRLF			   "\r\n"
#define BELL			   '\a'
#define COLON			   ':'
#define SPACE			   ' '
#define SPCL			   " :"
#define SPECIAL_CHARACTERS "[\\]^_'{|}"
#define NULCRLFSPCL		   "\0\r\n :"
#define NICK_DELAY		   30

/* Base reply ───────────────────────────────────────────────────────────────────────── */
#define RPL_BASE( client_, base_ )                                                          \
	COLON + client_._nickname + "!" + client_._username + "@" + client_._hostaddr + SPACE + \
		base_ + SPACE

/* Numeric Replies ──────────────────────────────────────────────────────────────────── */
#define NUMERIC_REPLY( num, nick ) \
	COLON + irc::server::__hostaddr + SPACE + num + SPACE + nick + SPCL
#define NUMERIC_REPLY_NOCL( num, nick ) \
	COLON + irc::server::__hostaddr + SPACE + num + SPACE + nick + SPACE

/* Error Replies ────────────────────────────────────────────────────────────────────── */

#define ERR_REPLY_BASE( num, client_ ) \
	NUMERIC_REPLY( num, client_._nickname ) + client_._message._command + SPCL

/* User Modes ───────────────────────────────────────────────────────────────────────── */

#define UMODES_AVAIL "awiro"
enum user_modes {
	UMODE_AWAY		 = 1,
	UMODE_WALLOPS	 = 2,
	UMODE_INVISIBLE	 = 4,
	UMODE_RESTRICTED = 8,
	UMODE_OPERATOR	 = 16
};

/* User Channel Modes ───────────────────────────────────────────────────────────────── */

#define UMODES_CHAN "ov"
enum user_channel_modes { UMODE_CHANOP = 1, UMODE_VOICE = 2, UMODE_CHANOWNER = 4 };

/* Channel Modes ────────────────────────────────────────────────────────────────────── */

#define CMODES_AVAIL "imnsptkl"
enum channel_modes {
	CMODE_INVITE	 = 1,
	CMODE_MODERATED	 = 2,
	CMODE_NOEXTERNAL = 4,
	CMODE_SECRET	 = 8,
	CMODE_PRIVATE	 = 16,
	CMODE_TOPIC		 = 32,
	CMODE_KEY		 = 64,
	CMODE_LIMIT		 = 128
};

/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */
/*                                      Exceptions                                      */
/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/* Throw Exception Macros ───────────────────────────────────────────────────────────── */

#define THROW_EXCEPT( str )	   throw std::runtime_error( str )
#define ERRCODE_EXCEPT( code ) THROW_EXCEPT( std::strerror( code ) )
#define ERRNO_EXCEPT		   ERRCODE_EXCEPT( errno )

/* Usage Exception Macros ───────────────────────────────────────────────────────────── */

#define ARGS_EXCEPT THROW_EXCEPT( "Usage: ./ircserv <port> <password>" )
#define PORT_EXCEPT THROW_EXCEPT( "<port> should be in range [1, 65535]" )
#define PASS_EXCEPT THROW_EXCEPT( "<password> shall not be empty" )

/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */
/*                                        Replies                                       */
/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

/* Define welcome messages for the client connection & registration success ─────────── */

#define RPL_WELCOME( client_ )                                                             \
	NUMERIC_REPLY( "001", client_._nickname ) + "Welcome to the Internet Relay Network " + \
		RPL_BASE( client_, "" ) + CRLF
#define RPL_YOURHOST( client_ )                                                       \
	NUMERIC_REPLY( "002", client_._nickname ) + "Your host is " + client_._hostaddr + \
		", running version 1.0 " + CRLF
#define RPL_CREATED( client_ )                                               \
	NUMERIC_REPLY( "003", client_._nickname ) + "This server was created " + \
		irc::server::__creationdate + " UTC+1 " + CRLF
#define RPL_MYINFO( client_ )                                                               \
	NUMERIC_REPLY( "004", client_._nickname ) + irc::server::__hostaddr + " v1.0" + SPACE + \
		UMODES_AVAIL + SPACE + CMODES_AVAIL + CRLF
#define REGISTRATION_SUCCESS( client_ )                                         \
	RPL_WELCOME( client_ ) + RPL_YOURHOST( client_ ) + RPL_CREATED( client_ ) + \
		RPL_MYINFO( client_ )

/* Define message of the day messages ───────────────────────────────────────────────── */

#define RPL_MOTDSTART( client_ )                                                                \
	NUMERIC_REPLY( "375", client_._nickname ) + "-" + SPACE + irc::server::__hostaddr + SPACE + \
		"Message of the day - " + CRLF
#define RPL_MOTD( client_, motd_ ) NUMERIC_REPLY( "372", client_._nickname ) + SPACE + motd_ + CRLF
#define RPL_ENDOFMOTD( client_ ) \
	NUMERIC_REPLY( "376", client_._nickname ) + "End of MOTD command" + CRLF
#define ASCII_ART( client_ )                                                                      \
	RPL_MOTD( client_, "" ) +                                                                     \
		RPL_MOTD( client_,                                                                        \
				  " /$$$$$$ /$$$$$$$   /$$$$$$   /$$$$$$  /$$$$$$$$ /$$$$$$$  /$$    /$$" ) +     \
		RPL_MOTD( client_,                                                                        \
				  "|_  $$_/| $$__  $$ /$$__  $$ /$$__  $$| $$_____/| $$__  $$| $$   | $$" ) +     \
		RPL_MOTD( client_,                                                                        \
				  "  | $$  | $$  \\ $$| $$  \\__/| $$  \\__/| $$      | $$  \\ $$| $$   | $$" ) + \
		RPL_MOTD( client_,                                                                        \
				  "  | $$  | $$$$$$$/| $$      |  $$$$$$ | $$$$$   | $$$$$$$/|  $$ / $$/" ) +     \
		RPL_MOTD( client_,                                                                        \
				  "  | $$  | $$__  $$| $$       \\____  $$| $$__/   | $$__  $$ \\  $$ $$/ " ) +   \
		RPL_MOTD( client_,                                                                        \
				  "  | $$  | $$  \\ $$| $$    $$ /$$  \\ $$| $$      | $$  \\ $$  \\  $$$/  " ) + \
		RPL_MOTD( client_,                                                                        \
				  " /$$$$$$| $$  | $$|  $$$$$$/|  $$$$$$/| $$$$$$$$| $$  | $$   \\  $/   " ) +    \
		RPL_MOTD( client_,                                                                        \
				  "|______/|__/  |__/ \\______/  \\______/ |________/|__/  |__/    \\_/    " ) +  \
		RPL_MOTD( client_, "" )

/* Define LUSER messages ────────────────────────────────────────────────────────────── */

#define RPL_LUSERCLIENT( client_ )                                                           \
	NUMERIC_REPLY( "251", client_._nickname ) + "There are " +                               \
		irc::server::__serv->getClientsSize() + " users and " +                              \
		irc::server::__serv->getChannelsSize() + " channels on " + irc::server::__hostaddr + \
		" Max: " + MAX_USERS + CRLF
#define RPL_LUSEROP( client_ )                                                        \
	NUMERIC_REPLY( "252", client_._nickname ) + irc::server::__serv->getOpersSize() + \
		" :operator(s) online" + CRLF
#define RPL_LUSERCHANNELS( client_ )                                                     \
	NUMERIC_REPLY( "254", client_._nickname ) + irc::server::__serv->getChannelsSize() + \
		" :channels formed" + CRLF
#define RPL_LUSERME( client_ )                              \
	NUMERIC_REPLY( "255", client_._nickname ) + "I have " + \
		irc::server::__serv->getClientsSize() + " clients and 0 servers" + CRLF

/* Define channel messages ──────────────────────────────────────────────────────────── */

#define RPL_CHANNELMODEIS( client_, channel_name, channel_modes ) \
	NUMERIC_REPLY_NOCL( "324", client_._nickname ) + channel_name + SPACE + channel_modes + CRLF

#define RPL_NOTOPIC( client_, channel_name ) \
	NUMERIC_REPLY_NOCL( "331", client_._nickname ) + channel_name + " :No topic is set" + CRLF
#define RPL_TOPIC( client_, channel_name, topic_ ) \
	NUMERIC_REPLY_NOCL( "332", client_._nickname ) + channel_name + SPCL + topic_ + CRLF
#define RPL_TOPIC_CHANGE( client_, channel_name, topic_ ) \
	RPL_BASE( client_, "TOPIC" ) + channel_name + SPCL + topic_ + CRLF

#define RPL_NAMREPLY( client_, channel_name, users_ )                                             \
	NUMERIC_REPLY_NOCL( "353", client_._nickname ) + "=" + SPACE + channel_name + SPCL + users_ + \
		CRLF
#define RPL_ENDOFNAMES( client_, channel_name ) \
	NUMERIC_REPLY_NOCL( "366", client_._nickname ) + channel_name + " :End of NAMES list" + CRLF

#define RPL_JOIN( newMember_, channel_name ) \
	RPL_BASE( client_, "JOIN" ) + COLON + channel_name + CRLF
#define RPL_UMODE( client_ ) \
	RPL_BASE( client_, "MODE" ) + client_._nickname + SPCL + client_.getModes() + CRLF
#define RPL_CHANMODE( client_, channel_name_, channel_modes_ ) \
	RPL_BASE( client_, "MODE" ) + channel_name_ + SPACE + channel_modes_ + CRLF
#define RPL_UMODEIS( client_ ) \
	NUMERIC_REPLY_NOCL( "221", client_._nickname ) + client_.getModes() + CRLF
#define RPL_YOUREOPER( client_ ) \
	NUMERIC_REPLY_NOCL( "381", client_._nickname ) + "You are now an IRC operator" + CRLF
#define RPL_INVITATION( client_, channel_name, target_nick_ ) \
	RPL_BASE( client_, "INVITE" ) + target_nick_ + " :" + channel_name + CRLF
#define RPL_INVITING( client_, channel_name, target_nick_ ) \
	NUMERIC_REPLY_NOCL( "341", client_._nickname ) + target_nick_ + SPACE + channel_name + CRLF
#define RPL_AWAY( client_, target_nick_, away_msg_ ) \
	NUMERIC_REPLY_NOCL( "301", client_._nickname ) + target_nick_ + SPCL + away_msg_ + CRLF

#define RPL_WHOISUSER( client_, target_ )                                         \
	NUMERIC_REPLY_NOCL( "311", client_._nickname ) + target_->_nickname + SPACE + \
		target_->_username + SPACE + target_->_hostaddr + SPACE + "*" + SPCL +    \
		target_->_realname + CRLF
#define RPL_WHOISSERVER( client_, target_ )                                       \
	NUMERIC_REPLY_NOCL( "312", client_._nickname ) + target_->_nickname + SPACE + \
		irc::server::__hostaddr + SPCL + "ircserv" + CRLF
#define RPL_WHOISOPERATOR( client_, target_ )                                                      \
	NUMERIC_REPLY_NOCL( "313", client_._nickname ) + target_->_nickname + " :is an IRC operator" + \
		CRLF
#define RPL_WHOISIDLE( client_, target_ )                                         \
	NUMERIC_REPLY_NOCL( "317", client_._nickname ) + target_->_nickname + SPACE + \
		target_->getIdleTime() + SPACE + target_->getSignOnTime() + SPCL +        \
		"seconds idle, signon time" + CRLF
#define RPL_ENDOFWHOIS( client_, target_ )                                                        \
	NUMERIC_REPLY_NOCL( "318", client_._nickname ) + target_->_nickname + " :End of WHOIS list" + \
		CRLF
#define RPL_WHOISCHANNELS( client_, target_ )                                    \
	NUMERIC_REPLY_NOCL( "319", client_._nickname ) + target_->_nickname + SPCL + \
		target_->getChannels( ' ' ) + CRLF
#define RPL_UNAWAY( client_ ) \
	NUMERIC_REPLY_NOCL( "305", client_._nickname ) + "You are no longer marked as being away" + CRLF
#define RPL_NOWAWAY( client_ ) \
	NUMERIC_REPLY_NOCL( "306", client_._nickname ) + "You have been marked as being away" + CRLF
#define RPL_NICKNAME( client_, new_nick_ ) RPL_BASE( client_, "NICK" ) + new_nick_ + CRLF
#define RPL_WHOREPLY( client_, channel_ptr_, other_ )                                           \
	NUMERIC_REPLY_NOCL( "352", client_._nickname ) + channel_ptr_->_name + SPACE +              \
		other_._username + SPACE + other_._hostaddr + SPACE + irc::server::__hostaddr + SPACE + \
		other_._nickname + SPACE + other_.who( *channel_ptr_ ) + SPCL + '0' + SPACE +           \
		other_._realname + CRLF
#define RPL_WHOREPLYNAME( client_, channel_name_, other_ )                                      \
	NUMERIC_REPLY_NOCL( "352", client_._nickname ) + channel_name_ + SPACE + other_._username + \
		SPACE + other_._hostaddr + SPACE + irc::server::__hostaddr + SPACE + other_._nickname + \
		SPACE + other_.who( channel_name_ ) + SPCL + '0' + SPACE + other_._realname + CRLF
#define RPL_WHOREPLY_PTR( client_, channel_ptr_, other_ptr_ )                                     \
	NUMERIC_REPLY_NOCL( "352", client_._nickname ) + channel_ptr_->_name + SPACE +                \
		other_ptr_->_username + SPACE + other_ptr_->_hostaddr + SPACE + irc::server::__hostaddr + \
		SPACE + other_ptr_->_nickname + SPACE + other_ptr_->who( *channel_ptr_ ) + SPCL + '0' +   \
		SPACE + other_ptr_->_realname + CRLF
#define RPL_ENDOFWHO( client_ ) \
	NUMERIC_REPLY_NOCL( "315", client_._nickname ) + COLON + "End of WHO list" + CRLF
#define RPL_BOT( client_, reply_ ) NUMERIC_REPLY_NOCL( "335", client_._nickname ) + reply_ + CRLF
#define RPL_RESTART( client_ ) \
	NUMERIC_REPLY_NOCL( "382", client_._nickname ) + "ircserv" + SPCL + "Restarting server" + CRLF
#define RPL_DIE( client_ ) \
	NUMERIC_REPLY_NOCL( "383", client_._nickname ) + "ircserv" + SPCL + "Server terminating" + CRLF
#define RPL_QUIT( client_ ) RPL_BASE( client_, "QUIT" ) + COLON + client_._away_msg + CRLF
#define RPL_CLOSINGLINK( client_ ) \
	"ERROR :Closing Link: " + client_._hostaddr + " (Quit: " + client_._away_msg + ")" + CRLF

/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */
/*                                        Errors                                        */
/* ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ */

#define ERR_NOSUCHSERVER( client_, server_ ) \
	ERR_REPLY_BASE( "402", client_ ) + server_ + SPCL + "No such server" + CRLF
#define ERR_NOSUCHCHANNEL( client_, channel_name ) \
	ERR_REPLY_BASE( "403", client_ ) + channel_name + " :No such channel" + CRLF
#define ERR_CANNOTSENDTOCHAN( client_, channel_name )                                          \
	ERR_REPLY_BASE( "404", client_ ) + channel_name +                                          \
		" :Cannot send messages to channel whilst the +m (moderated) mode without +v (voice) " \
		"mode" +                                                                               \
		CRLF
#define ERR_CHANNOEXT( client_, channel_name )                                              \
	ERR_REPLY_BASE( "404", client_ ) + channel_name +                                       \
		" :Cannot send external messages to this channel whilst the +n (noextmsg) mode is " \
		"set" +                                                                             \
		CRLF
#define ERR_TOOMANYCHANNELS( client_, channel_name ) \
	ERR_REPLY_BASE( "405", client_ ) + channel_name + " :You have joined too many channels" + CRLF
#define ERR_NOORIGIN( client_ )		   ERR_REPLY_BASE( "409", client_ ) + "No origin specified" + CRLF
#define ERR_NORECIPIENT( client_ )	   ERR_REPLY_BASE( "411", client_ ) + "No recipient given" + CRLF
#define ERR_NOTEXTTOSEND( client_ )	   ERR_REPLY_BASE( "412", client_ ) + "No text to send" + CRLF
#define ERR_NOCOMMANDGIVEN( client_ )  ERR_REPLY_BASE( "421", client_ ) + "No command given" + CRLF
#define ERR_UNKNOWNCOMMAND( client_ )  ERR_REPLY_BASE( "421", client_ ) + "Unknown command" + CRLF
#define ERR_NONICKNAMEGIVEN( client_ ) ERR_REPLY_BASE( "431", client_ ) + "No nickname given" + CRLF
#define ERR_USERONCHANNEL( client_, channel_name )                                       \
	NUMERIC_REPLY( "443", client_._nickname ) + client_._nickname + " " + channel_name + \
		" :is already on channel" + CRLF
#define ERR_NEEDMOREPARAMS( client_ ) \
	ERR_REPLY_BASE( "461", client_ ) + "Not enough parameters" + CRLF
#define ERR_MODELIMITNEEDMOREPARAMS( client_ ) \
	NUMERIC_REPLY_NOCL( "461", client_._nickname ) + "MODE +l :Not enough parameters" + CRLF
#define ERR_ALREADYREGISTRED( client_ ) \
	ERR_REPLY_BASE( "462", client_ ) + "Unauthorized command (already registered)" + CRLF
#define ERR_PASSWDMISMATCH( client_ ) ERR_REPLY_BASE( "464", client_ ) + "Password incorrect" + CRLF
#define ERR_KEYSET( client_, channel_name ) \
	ERR_REPLY_BASE( "467", client_ ) + channel_name + " :Channel key already set" + CRLF
#define ERR_UNKNOWNMODE( client_, mode_ ) \
	NUMERIC_REPLY_NOCL( "472", client_._nickname ) + mode_ + " :is unknown mode char to me" + CRLF
#define ERR_ERRONEUSNICKNAME( client_ ) \
	ERR_REPLY_BASE( "432", client_ ) + "Erroneous nickname" + CRLF
#define ERR_NICKNAMEINUSE( client_ ) \
	ERR_REPLY_BASE( "433", client_ ) + "Nickname is already in use" + CRLF
#define ERR_NICKCOLLISION( client_ ) ERR_REPLY_BASE( "436", client_ ) + "Nickname collision" + CRLF
#define ERR_UNAVAILRESOURCE( client_, time_left )                   \
	ERR_REPLY_BASE( "437", client_ ) + "Please wait " + time_left + \
		" seconds before attempting to change your nickname again." + CRLF
#define ERR_CHANNELISFULL( client_, channel_name ) \
	ERR_REPLY_BASE( "471", client_ ) + channel_name + " :Cannot join channel (+l)" + CRLF
#define ERR_BADCHANNELKEY( client_, channel_name ) \
	ERR_REPLY_BASE( "475", client_ ) + channel_name + " :Cannot join channel (+k)" + CRLF
#define RPL_LIST( client_, channel_name, users, topic )                                     \
	NUMERIC_REPLY_NOCL( "322", client_._nickname ) + channel_name + SPACE + users + SPACE + \
		topic + CRLF
#define RPL_LISTEND( client_ )		  NUMERIC_REPLY_NOCL( "323", client_._nickname ) + "End of LIST" + CRLF
#define ERR_TOOMANYMATCHES( client_ ) ERR_REPLY_BASE( "416", client_ ) + " :Too many matches" + CRLF
#define ERR_TOOMANYMATCHESNAMES( client_, channel_name_ ) \
	ERR_REPLY_BASE( "416", client_ ) + channel_name_ + " :Too many matches" + CRLF
#define ERR_INVITEONLYCHAN( client_, channel_name ) \
	ERR_REPLY_BASE( "473", client_ ) + channel_name + " :Cannot join channel (+i)" + CRLF
#define ERR_CHANOPRIVSNEEDED( client_, channel_name ) \
	ERR_REPLY_BASE( "482", client_ ) + channel_name + " :You're not channel operator" + CRLF
#define ERR_RESTRICTED( client_ ) \
	ERR_REPLY_BASE( "484", client_ ) + "Your connection is restricted!" + CRLF
#define ERR_USERNOTINCHANNEL( client_, absent_, channel_name )        \
	ERR_REPLY_BASE( "441", client_ ) + absent_ + " " + channel_name + \
		" :They aren't on that channel" + CRLF
#define ERR_NORECIPIENT( client_ )	ERR_REPLY_BASE( "411", client_ ) + "No recipient given" + CRLF
#define ERR_NOTEXTTOSEND( client_ ) ERR_REPLY_BASE( "412", client_ ) + "No text to send" + CRLF
#define ERR_NOSUCHNICK( client_, nickname_ ) \
	ERR_REPLY_BASE( "401", client_ ) + nickname_ + " :No such nick/channel" + CRLF
#define ERR_NOTONCHANNEL( client_, channel_name ) \
	ERR_REPLY_BASE( "442", client_ ) + channel_name + " :You're not on that channel" + CRLF
#define ERR_USERSDONTMATCH( client_ ) \
	ERR_REPLY_BASE( "502", client_ ) + "Can't change mode for other users" + CRLF
#define ERR_RESTRICTED( client_ ) \
	ERR_REPLY_BASE( "484", client_ ) + "Your connection is restricted!" + CRLF
#define ERR_NOPRIVILEGES( client_ ) \
	ERR_REPLY_BASE( "481", client_ ) + "Permission Denied- You're not an IRC operator" + CRLF

/* PRIVMSG ──────────────────────────────────────────────────────────────────────────── */

#define MSG( client_, target_, message_ ) \
	RPL_BASE( client_, "PRIVMSG" ) + target_ + SPCL + message_ + CRLF
#define NOTICE_MSG( client_, target_, message_ ) \
	RPL_BASE( client_, "NOTICE" ) + target_ + SPCL + message_ + CRLF

/* PARTMSG ──────────────────────────────────────────────────────────────────────────── */

#define RPL_PART( client_, channel_name ) RPL_BASE( client_, "PART" ) + channel_name + CRLF
#define RPL_PARTMSG( client_, channel_name, part_msg_ ) \
	RPL_BASE( client_, "PART" ) + channel_name + SPCL + part_msg_ + CRLF
