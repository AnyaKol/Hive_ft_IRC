#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace IRC {
	// ==========================================
	// Server Constants & Configuration
	// ==========================================
	constexpr std::uint16_t PORT_MIN			= 1024; // Below 1024 are privileged ports; 65535 is maximum value for uint16_t
	constexpr std::size_t	NICKLEN			  	= 9;
	constexpr std::size_t	USERLEN			  	= 9;
	constexpr std::size_t	CHANNELLEN			= 50;
	constexpr std::size_t	EVENT_QUEUE_SIZE	= 64;
	constexpr std::size_t	MAX_CHANNEL_SIZE	= 200;
	constexpr std::size_t	MODES				= 4; // max modes per MODE command

	inline const std::string SERVER_NAME				= "ircserv";
	inline const std::string SERVER_VERSION				= "1.0";
	inline const std::string AVAILABLE_USER_MODES		= "";
	inline const std::string AVAILABLE_CHANNEL_MODES	= "itkol";

	inline const std::string CHANTYPES			= "#"; // only regular channels
	inline const std::string PREFIX				= "(o)@"; // membership prefix: operator
	inline const std::string CASEMAPPING		= "rfc1459"; // names are ASCII case-insensitive plus `{}
	inline const std::size_t TARGMAX_PRIVMSG	= 4;   // max comma-separated targets per PRIVMSG
	inline const std::string TARGMAX			= "NAMES:1,TOPIC:1,KICK:1,PRIVMSG=" + std::to_string(TARGMAX_PRIVMSG);

	// Space-joined token list sent in RPL_ISUPPORT (005)
	inline const std::string ISUPPORT_TOKENS =
		"CHANTYPES=" + CHANTYPES
		+ " CHANNELLEN=" + std::to_string(CHANNELLEN)
		+ " NICKLEN=" + std::to_string(NICKLEN)
		+ " MAXCHANNELS=" + std::to_string(MAX_CHANNEL_SIZE)
		+ " PREFIX=" + PREFIX
		+ " CASEMAPPING=" + CASEMAPPING
		+ " MODES=" + std::to_string(MODES)
		+ " TARGMAX=" + TARGMAX	;

	// Split a comma-separated parameter (e.g. "bob,#chan" from PRIVMSG) into parts
	inline std::vector<std::string_view> splitByCommas(const std::string_view& list) {
		std::vector<std::string_view> parts;
		std::size_t start = 0;
		while (true) {
			std::size_t pos = list.find(',', start);
			if (pos == std::string_view::npos) {
				parts.push_back(list.substr(start));
				break;
			}
			parts.push_back(list.substr(start, pos - start));
			start = pos + 1;
		}
		return parts;
	}

	// ==========================================
	// Numeric Codes enum
	// ==========================================
	enum NumericCodes {
		//Reply
		RPL_WELCOME		 = 1, // Client completes PASS + NICK + USER.
		RPL_YOURHOST		= 2, // Server metadata.
		RPL_CREATED		 = 3, //
		RPL_MYINFO		  = 4, //
		RPL_ISUPPORT		= 5, // Tells the client channel prefixes (#, &) and mode limits.

		RPL_ENDOFWHOIS		= 318, //Responce to WHOIS command.
		RPL_CHANNELMODEIS	= 324, // Sent in response to MODE <channel> command, shows the current channel modes and their parameters.
		RPL_CREATIONTIME	= 329, // Sent in response to WHOIS <nick> command, shows the channel creation time.
		RPL_NOTOPIC		 = 331, // Sent in response to TOPIC <channel> command, indicates that the channel has no topic set.
		RPL_TOPIC			= 332, // Sent in response to TOPIC <channel> command, shows the current topic of the channel.
		RPL_TOPICWHOTIME	= 333, // Sent in response to WHOIS <nick> command, shows the user who set the topic and the time it was set.
		RPL_INVITING		= 341, // Sent in response to INVITE <nick> <channel> command, indicates that the user has been invited to the channel.
		RPL_NAMREPLY		= 353, // Sent in response to NAMES <channel> command, shows the list of users in the channel.
		RPL_ENDOFNAMES	  = 366, // Sent in response to NAMES <channel> command, indicates the end of the names list.

		//Errors
		ERR_NOSUCHNICK		= 401, // Sent in response to a command that requires a target nick, but the specified nick does not exist.
		ERR_NOSUCHCHANNEL	= 403, // Sent in response to a command that requires a target channel, but the specified channel does not exist.
		ERR_CANNOTSENDTOCHAN = 404, //
		ERR_NOORIGIN		 = 409, // Sent in response to a command that requires an origin, but no origin is specified.
		ERR_NORECIPIENT	  = 411, // Sent in response to a command that requires a recipient, but no recipient is specified.
		ERR_NOTEXTTOSEND	 = 412, // Sent in response to a command that requires text to be sent, but no text is specified.
		ERR_UNKNOWNCOMMAND	= 421, // Sent in response to a command that is not recognized by the server.
		ERR_NONICKNAMEGIVEN  = 431, // Sent in response to a command that requires a nick, but no nick is specified.
		ERR_ERRONEUSNICKNAME = 432, // Sent in response to a command that requires a valid nick, but the specified nick is invalid.
		ERR_NICKNAMEINUSE	= 433, // Sent in response to a command that requires a unique nick, but the specified nick is already in use.
		ERR_USERNOTINCHANNEL = 441, // Sent in response to a command that requires the user to be in a channel, but the user is not in the channel.
		ERR_NOTONCHANNEL	 = 442, //
		ERR_USERONCHANNEL	= 443, // Sent in response to a command that requires the user to be in a channel, but the user is already in the channel.

		ERR_NOTREGISTERED	= 451, // Sent in response to a command that requires the user to be registered, but the user is not registered.
		ERR_NEEDMOREPARAMS	= 461, // Sent in response to a command that requires more parameters, but not enough are specified.
		ERR_ALREADYREGISTERED= 462, // Sent in response to a command that requires the user to be registered, but the user is already registered.
		ERR_PASSWDMISMATCH	= 464, // Sent in response to a command that requires a password, but the specified password is incorrect.
		ERR_CHANNELISFULL	= 471, // Sent in response to a command that requires the channel to be not full, but the channel is full.
		ERR_UNKNOWNMODE	  = 472, // Sent in response to a command that requires a known mode, but the specified mode is unknown.
		ERR_INVITEONLYCHAN	= 473, // Sent in response to a command that requires the channel to be invite-only, but the user is not invited.
		ERR_BADCHANNELKEY	= 475, // Sent in response to a command that requires a valid channel key, but the specified key is invalid.
		ERR_BADCHANMASK	  = 476, // Sent in response to a command that requires a valid channel mask, but the specified mask is invalid.
		ERR_CHANOPRIVSNEEDED = 482, // Sent in response to a command that requires the user to be a channel operator, but the user is not an operator.

		ERR_INVALIDKEY		= 525, // Sent in response to a command that requires a valid key, but the specified key is not well-formed.
		ERR_INVALIDMODEPARAM = 696, // Sent in response to a command that requires a valid mode parameter, but the specified parameter is invalid.
	};

	// ==========================================
	// RFC-Compliant Reply Builders
	// Format: :<server> <code> <nick> <params...> :<message>\r\n
	// ==========================================
	namespace Reply
	{
		inline std::string target(std::string_view nick)
		{
			return nick.empty() ? "*" : std::string(nick);
		}

		// Generic reply builder
		inline std::string build(std::string_view code, std::string_view nick, std::string_view params, std::string_view trailing)
		{
			std::string msg = ":" + std::string(SERVER_NAME) + " " + std::string(code) + " " + target(nick);
			if (!params.empty())
				msg += " " + std::string(params);
			if (!trailing.empty())
				msg += " :" + std::string(trailing);
			msg += "\r\n";
			return msg;
		}

		// --- Registration & Connection Handshake ---
		inline std::string welcome(std::string_view nick, std::string_view user, std::string_view host)
		{
			static_assert(RPL_WELCOME == 1, "001 literal must match RPL_WELCOME");
			return ":" + std::string(SERVER_NAME) + " 001 " + target(nick) +
				" :Welcome to the IRC Network, " + std::string(nick) + "!" + std::string(user) + "@" + std::string(host) + "\r\n";
		}

		inline std::string yourHost(std::string_view nick)
		{
			static_assert(RPL_YOURHOST == 2, "002 literal must match RPL_YOURHOST");
			return ":" + std::string(SERVER_NAME) + " 002 " + target(nick) +
				" :Your host is " + std::string(SERVER_NAME) + ", running version " + std::string(SERVER_VERSION) + "\r\n";
		}

		inline std::string created(std::string_view nick, std::string_view datetime)
		{
			static_assert(RPL_CREATED == 3, "003 literal must match RPL_CREATED");
			return ":" + std::string(SERVER_NAME) + " 003 " + target(nick) +
				" :This server was created " + std::string(datetime) + "\r\n";
		}

		inline std::string myInfo(std::string_view nick)
		{
			static_assert(RPL_MYINFO == 4, "004 literal must match RPL_MYINFO");
			return ":" + std::string(SERVER_NAME) + " 004 " + target(nick) + " " +
				std::string(SERVER_NAME) + " " + std::string(SERVER_VERSION) + " " +
				std::string(AVAILABLE_USER_MODES) + " " + std::string(AVAILABLE_CHANNEL_MODES) + "\r\n";
		}

		inline std::string isSupport(std::string_view nick, std::string_view tokens)
		{
			static_assert(RPL_ISUPPORT == 5, "005 literal must match RPL_ISUPPORT");
			return ":" + std::string(SERVER_NAME) + " 005 " + target(nick) + " " +
				std::string(tokens) + " :are supported by this server\r\n";
		}


		// --- Channel Info & Actions ---
		inline std::string endOfWhoIs(std::string_view nick, std::string_view targetNick)
		{
			static_assert(RPL_ENDOFWHOIS == 318, "318 literal must match RPL_ENDOFWHOIS");
			return ":" + std::string(SERVER_NAME) + " 318 " + target(nick) + " " + std::string(targetNick) + " :End of /WHOIS list\r\n";
		}


		inline std::string channelModeIs(std::string_view nick, std::string_view channel, std::string_view modes, std::string_view params = "")
		{
			static_assert(RPL_CHANNELMODEIS == 324, "324 literal must match RPL_CHANNELMODEIS");
			std::string res = ":" + std::string(SERVER_NAME) + " 324 " + target(nick) + " " + std::string(channel) + " " + std::string(modes);
			if (!params.empty())
				res += " " + std::string(params);
			res += "\r\n";
			return res;
		}

		inline std::string creationTime(std::string_view nick, std::string_view channel, std::string_view timestamp)
		{
			static_assert(RPL_CREATIONTIME == 329, "329 literal must match RPL_CREATIONTIME");
			return ":" + std::string(SERVER_NAME) + " 329 " + target(nick) + " " + std::string(channel) + " " + std::string(timestamp) + "\r\n";
		}

		inline std::string noTopic(std::string_view nick, std::string_view channel)
		{
			static_assert(RPL_NOTOPIC == 331, "331 literal must match RPL_NOTOPIC");
			return ":" + std::string(SERVER_NAME) + " 331 " + target(nick) + " " + std::string(channel) + " :No topic is set\r\n";
		}

		inline std::string topic(std::string_view nick, std::string_view channel, std::string_view topicText)
		{
			static_assert(RPL_TOPIC == 332, "332 literal must match RPL_TOPIC");
			return ":" + std::string(SERVER_NAME) + " 332 " + target(nick) + " " + std::string(channel) + " :" + std::string(topicText) + "\r\n";
		}

		inline std::string topicWhoTime(std::string_view nick, std::string_view channel, std::string_view setter, std::string_view timestamp)
		{
			static_assert(RPL_TOPICWHOTIME == 333, "333 literal must match RPL_TOPICWHOTIME");
			return ":" + std::string(SERVER_NAME) + " 333 " + target(nick) + " " + std::string(channel) + " " + std::string(setter) + " " + std::string(timestamp) + "\r\n";
		}

		inline std::string inviting(std::string_view nick, std::string_view targetNick, std::string_view channel)
		{
			static_assert(RPL_INVITING == 341, "341 literal must match RPL_INVITING");
			return ":" + std::string(SERVER_NAME) + " 341 " + target(nick) + " " + std::string(targetNick) + " " + std::string(channel) + "\r\n";
		}

		/* Joins operator nicks (prefixed with '@') and member nicks into the
		 * space-separated body used as the trailing param of namReply. */
		inline std::string namesList(const std::vector<std::string>& opNicks,
			const std::vector<std::string>& memNicks)
		{
			std::string	nameList;
			for (const std::string& nick : opNicks) {
				if (!nameList.empty())
					nameList += " ";
				nameList += "@" + nick;
			}
			for (const std::string& nick : memNicks) {
				if (!nameList.empty())
					nameList += " ";
				nameList += nick;
			}
			return (nameList);
		}

		inline std::string namReply(std::string_view nick, std::string_view channel, std::string_view namesList)
		{
			static_assert(RPL_NAMREPLY == 353, "353 literal must match RPL_NAMREPLY");
			return ":" + std::string(SERVER_NAME) + " 353 " + target(nick) + " = " + std::string(channel) + " :" + std::string(namesList) + "\r\n";
		}

		inline std::string endOfNames(std::string_view nick, std::string_view channel)
		{
			static_assert(RPL_ENDOFNAMES == 366, "366 literal must match RPL_ENDOFNAMES");
			return ":" + std::string(SERVER_NAME) + " 366 " + target(nick) + " " + std::string(channel) + " :End of /NAMES list\r\n";
		}

		// --- Error Replies ---
		inline std::string errNoSuchNick(std::string_view nick, std::string_view targetNick)
		{
			static_assert(ERR_NOSUCHNICK == 401, "401 literal must match ERR_NOSUCHNICK");
			return ":" + std::string(SERVER_NAME) + " 401 " + target(nick) + " " + std::string(targetNick) + " :No such nick/channel\r\n";
		}

		inline std::string errNoSuchChannel(std::string_view nick, std::string_view channel)
		{
			static_assert(ERR_NOSUCHCHANNEL == 403, "403 literal must match ERR_NOSUCHCHANNEL");
			return ":" + std::string(SERVER_NAME) + " 403 " + target(nick) + " " + std::string(channel) + " :No such channel\r\n";
		}

		inline std::string errCannotSendToChannel(std::string_view nick, std::string_view channel)
		{
			static_assert(ERR_CANNOTSENDTOCHAN == 404, "404 literal must match ERR_CANNOTSENDTOCHAN");
			return ":" + std::string(SERVER_NAME) + " 404 " + target(nick) + " " + std::string(channel) + " :Cannot send to channel\r\n";
		}

		inline std::string errNoOrigin(std::string_view nick)
		{
			static_assert(ERR_NOORIGIN == 409, "409 literal must match ERR_NOORIGIN");
			return ":" + std::string(SERVER_NAME) + " 409 " + target(nick) + " :No origin specified\r\n";
		}

		inline std::string errNoRecipient(std::string_view nick, std::string_view command)
		{
			static_assert(ERR_NORECIPIENT == 411, "411 literal must match ERR_NORECIPIENT");
			return ":" + std::string(SERVER_NAME) + " 411 " + target(nick) + " :No recipient given (" + std::string(command) + ")\r\n";
		}

		inline std::string errNoTextToSend(std::string_view nick)
		{
			static_assert(ERR_NOTEXTTOSEND == 412, "412 literal must match ERR_NOTEXTTOSEND");
			return ":" + std::string(SERVER_NAME) + " 412 " + target(nick) + " :No text to send\r\n";
		}

		inline std::string errUnknownCommand(std::string_view nick, std::string_view command)
		{
			static_assert(ERR_UNKNOWNCOMMAND == 421, "421 literal must match ERR_UNKNOWNCOMMAND");
			return ":" + std::string(SERVER_NAME) + " 421 " + target(nick) + " " + std::string(command) + " :Unknown command\r\n";
		}

		inline std::string errNoNickGiven(std::string_view nick)
		{
			static_assert(ERR_NONICKNAMEGIVEN == 431, "431 literal must match ERR_NONICKNAMEGIVEN");
			return ":" + std::string(SERVER_NAME) + " 431 " + target(nick) + " :No nickname given\r\n";
		}

		inline std::string errErroneousNick(std::string_view nick, std::string_view badNick)
		{
			static_assert(ERR_ERRONEUSNICKNAME == 432, "432 literal must match ERR_ERRONEUSNICKNAME");
			return ":" + std::string(SERVER_NAME) + " 432 " + target(nick) + " " + std::string(badNick) + " :Erroneous nickname\r\n";
		}

		inline std::string errNickInUse(std::string_view nick, std::string_view badNick)
		{
			static_assert(ERR_NICKNAMEINUSE == 433, "433 literal must match ERR_NICKNAMEINUSE");
			return ":" + std::string(SERVER_NAME) + " 433 " + target(nick) + " " + std::string(badNick) + " :Nickname is already in use\r\n";
		}

		inline std::string errUserNotInChannel(std::string_view nick, std::string_view targetNick, std::string_view channel)
		{
			static_assert(ERR_USERNOTINCHANNEL == 441, "441 literal must match ERR_USERNOTINCHANNEL");
			return ":" + std::string(SERVER_NAME) + " 441 " + target(nick) + " " + std::string(targetNick) + " " + std::string(channel) + " :They aren't on that channel\r\n";
		}

		inline std::string errNotOnChannel(std::string_view nick, std::string_view channel)
		{
			static_assert(ERR_NOTONCHANNEL == 442, "442 literal must match ERR_NOTONCHANNEL");
			return ":" + std::string(SERVER_NAME) + " 442 " + target(nick) + " " + std::string(channel) + " :You're not on that channel\r\n";
		}

		inline std::string errUserAlreadyInChannel(std::string_view nick, std::string_view targetNick, std::string_view channel)
		{
			static_assert(ERR_USERONCHANNEL == 443, "443 literal must match ERR_USERONCHANNEL");
			return ":" + std::string(SERVER_NAME) + " 443 " + target(nick) + " " + std::string(targetNick) + " " + std::string(channel) + " :is already on channel\r\n";
		}

		inline std::string errNotRegistered(std::string_view nick)
		{
			static_assert(ERR_NOTREGISTERED == 451, "451 literal must match ERR_NOTREGISTERED");
			return ":" + std::string(SERVER_NAME) + " 451 " + target(nick) + " :You have not registered\r\n";
		}

		inline std::string errNeedMoreParams(std::string_view nick, std::string_view command)
		{
			static_assert(ERR_NEEDMOREPARAMS == 461, "461 literal must match ERR_NEEDMOREPARAMS");
			return ":" + std::string(SERVER_NAME) + " 461 " + target(nick) + " " + std::string(command) + " :Not enough parameters\r\n";
		}

		inline std::string errAlreadyRegistered(std::string_view nick)
		{
			static_assert(ERR_ALREADYREGISTERED == 462, "462 literal must match ERR_ALREADYREGISTERED");
			return ":" + std::string(SERVER_NAME) + " 462 " + target(nick) + " :Unauthorized command (already registered)\r\n";
		}

		inline std::string errPasswordMismatch(std::string_view nick)
		{
			static_assert(ERR_PASSWDMISMATCH == 464, "464 literal must match ERR_PASSWDMISMATCH");
			return ":" + std::string(SERVER_NAME) + " 464 " + target(nick) + " :Password incorrect\r\n";
		}

		inline std::string errChannelIsFull(std::string_view nick, std::string_view channel)
		{
			static_assert(ERR_CHANNELISFULL == 471, "471 literal must match ERR_CHANNELISFULL");
			return ":" + std::string(SERVER_NAME) + " 471 " + target(nick) + " " + std::string(channel) + " :Cannot join channel (+l)\r\n";
		}

		inline std::string errUnknownMode(std::string_view nick, char modeChar)
		{
			static_assert(ERR_UNKNOWNMODE == 472, "472 literal must match ERR_UNKNOWNMODE");
			return ":" + std::string(SERVER_NAME) + " 472 " + target(nick) + " " + std::string(1, modeChar) + " :is unknown mode char to me\r\n";
		}

		inline std::string errInviteOnlyChannel(std::string_view nick, std::string_view channel)
		{
			static_assert(ERR_INVITEONLYCHAN == 473, "473 literal must match ERR_INVITEONLYCHAN");
			return ":" + std::string(SERVER_NAME) + " 473 " + target(nick) + " " + std::string(channel) + " :Cannot join channel (+i)\r\n";
		}

		inline std::string errBadChannelKey(std::string_view nick, std::string_view channel)
		{
			static_assert(ERR_BADCHANNELKEY == 475, "475 literal must match ERR_BADCHANNELKEY");
			return ":" + std::string(SERVER_NAME) + " 475 " + target(nick) + " " + std::string(channel) + " :Cannot join channel (+k)\r\n";
		}

		inline std::string errBadChannelMask(std::string_view nick, std::string_view channel)
		{
			static_assert(ERR_BADCHANMASK == 476, "476 literal must match ERR_BADCHANMASK");
			return ":" + std::string(SERVER_NAME) + " 476 " + target(nick) + " " + std::string(channel) + " :Bad Channel Mask\r\n";
		}

		inline std::string errChannelOpRequired(std::string_view nick, std::string_view channel)
		{
			static_assert(ERR_CHANOPRIVSNEEDED == 482, "482 literal must match ERR_CHANOPRIVSNEEDED");
			return ":" + std::string(SERVER_NAME) + " 482 " + target(nick) + " " + std::string(channel) + " :You're not channel operator\r\n";
		}

		inline std::string errInvalidKey(std::string_view nick, std::string_view channel)
		{
			static_assert(ERR_INVALIDKEY == 525, "525 literal must match ERR_INVALIDKEY");
			return ":" + std::string(SERVER_NAME) + " 525 " + target(nick) + " " + std::string(channel) + " :Key is not well-formed\r\n";
		}

		inline std::string errInvalidModeParam(std::string_view nick, std::string_view channel, char mode, std::string_view param, std::string_view desc)
		{
			static_assert(ERR_INVALIDMODEPARAM == 696, "696 literal must match ERR_INVALIDMODEPARAM");
			return ":" + std::string(SERVER_NAME) + " 696 " + target(nick) + " " + std::string(channel) + " " + std::string(1, mode) + " " + std::string(param) + " :" + std::string(desc) + "\r\n";
		}
	}
}

/* usage examples:
// When a regular user tries to KICK or change channel MODE:
std::string reply = IRC::Reply::errChannelOpRequired(client.getNickname(), channel.getName());
send(client.getSocket(), reply.c_str(), reply.length(), 0);

// When a password in PASS is incorrect:
std::string reply = IRC::Reply::errPasswordMismatch(client.getNickname());
send(client.getSocket(), reply.c_str(), reply.length(), 0);

// When trying to join an invite-only room (+i):
std::string reply = IRC::Reply::errInviteOnlyChannel(client.getNickname(), channel.getName());
send(client.getSocket(), reply.c_str(), reply.length(), 0);
 */