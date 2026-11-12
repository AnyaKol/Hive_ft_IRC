#include "Channel.hpp"
#include "Client.hpp"
#include "IRC.hpp"

// Channel commands
#include "Invite.hpp"
#include "Join.hpp"
#include "Kick.hpp"
#include "Mode.hpp"
#include "Names.hpp"
#include "Part.hpp"
#include "Topic.hpp"

#include <vector>
//For unique pointer
#include <memory>


// Set user limit to 0 instead of NAN because NAN is float
Channel::Channel(std::string_view name, Client& creator) {
	this->_name = name;
	this->_topic = "";
	this->_key = "";
	this->_userLimit = 0;
	this->addOperator(creator);
}


//Setters
void	Channel::setName(std::string_view name) {
	// Invalid name: keep the current one.
	if (!isValidChannelName(name)) {
		return;
	}
	this->_name = name;
}

void	Channel::setKey(std::string key, Client& client) {
	if (key.empty() || key[0] == '#' || key[0] == '&' || key == "x") {
		Channel::sendNumeric(client, IRC::ERR_INVALIDKEY, this->_name);
		return;
	}

	this->_key = std::move(key);
	this->_hasKey = true;
}

void	Channel::setTopic(std::string_view topic, Client& client) {
	if (this->_protectedTopic && !this->_operators.contains(&client)) {
		return;
	}
	this->_topic = topic;
}

void	Channel::setUserLimit(std::size_t limit) {
	if (limit == 0 || this->_members.size() + this->_operators.size() > limit) {
		return;
	}
	this->_userLimit = limit;
	this->_hasUserLimit = true;
}

/* Flag setters used by MODE */
void	Channel::setInviteOnly(bool value) {
	this->_inviteOnly = value;
}

void	Channel::setTopicProtected(bool value) {
	this->_protectedTopic = value;
}

void	Channel::clearKey(void) {
	this->_key.clear();
	this->_hasKey = false;
}

void	Channel::clearUserLimit(void) {
	this->_userLimit = 0;
	this->_hasUserLimit = false;
}

void	Channel::addMember(Client& client, std::string_view key) {
	if (this->isMember(client) || this->isOperator(client)) {
		// JOIN on a channel already joined: no need for a reply
		return;
	}
	if (this->_inviteOnly) {
		if ( !this->_pendingInvites.contains(client.getNickname()) ) {
			Channel::sendNumeric(client, IRC::ERR_INVITEONLYCHAN, this->_name, "Cannot join channel (+i)");
			return;
		}
	}
	if (this->_hasKey && this->_key != key) {
		// No key supplied or wrong key
		Channel::sendNumeric(client, IRC::ERR_BADCHANNELKEY, this->_name, "Cannot join channel (+k)");
		return;
	}
	if (this->isFull()) {
		Channel::sendNumeric(client, IRC::ERR_CHANNELISFULL, this->_name, "Cannot join channel (+l)");
		return;
	}

	// Add member
	this->_members.insert(&client);

	if (this->_inviteOnly)
		this->_pendingInvites.erase(this->_pendingInvites.find(client.getNickname())); //Remove from invites
}

void	Channel::addOperator(Client& client) {
	// The creator path bypasses this because a fresh channel is never full.
	if (this->isFull())
		return;
	this->_operators.insert(&client);
}

void	Channel::removeMember(Client& client) {
	auto it = this->_members.find(&client);
	if (it != this->_members.end())
		this->_members.erase(it);
}

void	Channel::removeOperator(Client& client) {
	auto it = this->_operators.find(&client);
	if (it != this->_operators.end())
		this->_operators.erase(it);
}

/* Removes the client from members and operators and broadcasts a QUIT-style
 * message. Returns true if the client was on this channel. */
bool	Channel::removeClientEverywhere(Client& client, const std::string& reason) {
	bool	wasHere = this->isMember(client) || this->isOperator(client);

	if (!wasHere)
		return (false);

	// Create custom user message
	std::string	quitMsg = ":" + client.getNickname() + "!" + client.getUsername() +
		"@" + client.getHostname() + " QUIT :Quit: " + reason + "\r\n";
	// Sender doesn't get the message
	this->publicMessage(quitMsg, &client);

	this->removeMember(client);
	this->removeOperator(client);
	return (true);
}

// Adds nickname to pending invites unless already invited
bool	Channel::invite(const std::string& nickname) {
	if (this->_pendingInvites.contains(nickname))
		return (false);
	this->_pendingInvites.insert(nickname);
	return (true);
}

void	Channel::memberToOperator(Client& client) {
	if (this->isOperator(client))
		return;
	this->removeMember(client);
	this->_operators.insert(&client);
}

void	Channel::operatorToMember(Client& client) {
	if (this->isMember(client))
		return;
	this->removeOperator(client);
	this->_members.insert(&client);
}

bool	Channel::isMember(Client& client) const {
	return (this->_members.contains(&client));
}

bool	Channel::isOperator(Client& client) const {
	return (this->_operators.contains(&client));
}

bool	Channel::isFull(void) const {
	if (!this->_hasUserLimit)
		return (false);

	if (this->_members.size() + this->_operators.size() >= this->_userLimit)
		return (true);
	return (false);
}

Client*	Channel::getClientByNick(const std::string_view& nickname) const {
	// Check members 
	for (Client* client : this->_members) {
		if (client->getNickname() == nickname) {
			return (client);
		}
	}
	// Check operators
	for (Client* client : this->_operators) {
		if (client->getNickname() == nickname) {
			return (client);
		}
	}
	return nullptr;
}

/* Channel name may not contain any spaces (' ', 0x20), a control G / BELL
 * ('^G', 0x07), or a comma (',', 0x2C)
 * Channel name prefix may be:
 * 	('#', 0x23) - regular channel; known to all servers that are connected to
 * 	the network
 * 	('&', 0x26) - local channels; the clients connected can only see and talk
 * 	to other clients on the same server
 */
bool	Channel::isValidChannelName(std::string_view name) {
	if (name.size() < 2 || name.size() > IRC::CHANNELLEN)
		return false;
	if (name[0] != '#' && name[0] != '&')
		return false;
	for (char c : name) {
		if (c == ' ' || c == ',' || c == '\a' || static_cast<unsigned char>(c) <= 32)
			return false;
	}
	return true;
}

void	Channel::publicMessage(const std::string& msg, const Client* sender) const {
	if (msg.size() == 0)
		return;

	// Send to every operator and member except the sender
	for (Client* client : this->_operators) {
		if (client != sender)
			client->appendToWriteBuffer(msg);
	}
	for (Client* client : this->_members) {
		if (client != sender)
			client->appendToWriteBuffer(msg);
	}
}

/* Routes a numeric to its IRC::Reply builder and queues the formatted reply.
 * params:   middle parameters (space-separated), without the nick
 * trailing: text sent after ':' (may contain spaces) */
void	Channel::sendNumeric(Client& client, IRC::NumericCodes code,
	const std::string& params, const std::string& trailing)
{
	const std::string&	nick = client.getNickname();
	std::string	msg;

	switch (code) {
		case IRC::RPL_CHANNELMODEIS:	msg = IRC::Reply::channelModeIs(nick, params, trailing); break;
		case IRC::RPL_CREATIONTIME:		msg = IRC::Reply::creationTime(nick, params, trailing); break;
		case IRC::RPL_NOTOPIC:			msg = IRC::Reply::noTopic(nick, params); break;
		case IRC::RPL_TOPIC:			msg = IRC::Reply::topic(nick, params, trailing); break;
		case IRC::RPL_TOPICWHOTIME:		msg = IRC::Reply::topicWhoTime(nick, params, params, trailing); break;
		case IRC::RPL_INVITING:			msg = IRC::Reply::inviting(nick, params, trailing); break;
		case IRC::RPL_NAMREPLY:			msg = IRC::Reply::namReply(nick, params, trailing); break;
		case IRC::RPL_ENDOFNAMES:		msg = IRC::Reply::endOfNames(nick, params); break;
		case IRC::ERR_NOSUCHNICK:		msg = IRC::Reply::errNoSuchNick(nick, params); break;
		case IRC::ERR_NOSUCHCHANNEL:	msg = IRC::Reply::errNoSuchChannel(nick, params); break;
		case IRC::ERR_CANNOTSENDTOCHAN:	msg = IRC::Reply::errCannotSendToChannel(nick, params); break;
		case IRC::ERR_NOORIGIN:			msg = IRC::Reply::errNoOrigin(nick); break;
		case IRC::ERR_NORECIPIENT:		msg = IRC::Reply::errNoRecipient(nick, params); break;
		case IRC::ERR_NOTEXTTOSEND:		msg = IRC::Reply::errNoTextToSend(nick); break;
		case IRC::ERR_UNKNOWNCOMMAND:	msg = IRC::Reply::errUnknownCommand(nick, params); break;
		case IRC::ERR_NONICKNAMEGIVEN:	msg = IRC::Reply::errNoNickGiven(nick); break;
		case IRC::ERR_ERRONEUSNICKNAME:	msg = IRC::Reply::errErroneousNick(nick, params); break;
		case IRC::ERR_NICKNAMEINUSE:	msg = IRC::Reply::errNickInUse(nick, params); break;
		case IRC::ERR_USERNOTINCHANNEL:	msg = IRC::Reply::errUserNotInChannel(nick, params, trailing); break;
		case IRC::ERR_NOTONCHANNEL:		msg = IRC::Reply::errNotOnChannel(nick, params); break;
		case IRC::ERR_USERONCHANNEL:	msg = IRC::Reply::errUserAlreadyInChannel(nick, params, trailing); break;
		case IRC::ERR_NOTREGISTERED:	msg = IRC::Reply::errNotRegistered(nick); break;
		case IRC::ERR_NEEDMOREPARAMS:	msg = IRC::Reply::errNeedMoreParams(nick, params); break;
		case IRC::ERR_ALREADYREGISTERED:	msg = IRC::Reply::errAlreadyRegistered(nick); break;
		case IRC::ERR_PASSWDMISMATCH:	msg = IRC::Reply::errPasswordMismatch(nick); break;
		case IRC::ERR_CHANNELISFULL:	msg = IRC::Reply::errChannelIsFull(nick, params); break;
		case IRC::ERR_UNKNOWNMODE:		msg = IRC::Reply::errUnknownMode(nick, trailing.empty() ? '?' : trailing[0]); break;
		case IRC::ERR_INVITEONLYCHAN:	msg = IRC::Reply::errInviteOnlyChannel(nick, params); break;
		case IRC::ERR_BADCHANNELKEY:	msg = IRC::Reply::errBadChannelKey(nick, params); break;
		case IRC::ERR_BADCHANMASK:		msg = IRC::Reply::errBadChannelMask(nick, params); break;
		case IRC::ERR_CHANOPRIVSNEEDED:	msg = IRC::Reply::errChannelOpRequired(nick, params); break;
		case IRC::ERR_INVALIDKEY:		msg = IRC::Reply::errInvalidKey(nick, params); break;
		case IRC::ERR_INVALIDMODEPARAM:	msg = IRC::Reply::errInvalidModeParam(nick, params, '?', "", trailing); break;
		default:						return; // unknown numeric: send nothing
	}
	client.appendToWriteBuffer(msg);
}

// Commands
bool	Channel::isChannelCommand(Server& server,
			std::unordered_map<std::string, Channel>& channels,
			Client& client, const std::string& command,
			const std::vector<std::string_view>& params) {

	try {
		std::unique_ptr<AChannelCommand> cmd( makeCommand(server, command, client, channels) );
		cmd->execute(params);
	} catch (Channel::Exception &e) { // Expected command failures (bad params, not a channel command)
		return (false);
	} catch (Channel::NameException &e) { // Channel lookup/creation failures
		return (false);
	}
	return (true);
}

AChannelCommand*	Channel::makeCommand(Server& server, const std::string& command, Client& client,
	std::unordered_map<std::string, Channel>& channels) {

	enum		allCommands {
		INVITE,
		JOIN,
		KICK,
		MODE,
		NAMES,
		PART,
		TOPIC
	};
	int commandNumber = makeCommandNumber(command);

	switch (commandNumber) {
		case allCommands::INVITE:
			return (new Invite(client, channels, server));
		case allCommands::JOIN:
			return (new Join(client, channels, server));
		case allCommands::KICK:
			return (new Kick(client, channels, server));
		case allCommands::MODE:
			return (new Mode(client, channels, server));
		case allCommands::NAMES:
			return (new Names(client, channels, server));
		case allCommands::PART:
			return (new Part(client, channels, server));
		case allCommands::TOPIC:
			return (new Topic(client, channels, server));
		default:
			throw (Exception(ERR_NOT_CHANNEL_COMMAND));
	}
}

int	Channel::makeCommandNumber(const std::string& command) {
	const std::vector<std::string>	commandStrings = {
		"INVITE",
		"JOIN",
		"KICK",
		"MODE",
		"NAMES",
		"PART",
		"TOPIC"
	};
	for (int num = 0; num < static_cast<int>(commandStrings.size()); num++) {
		if (command == commandStrings[num])
			return (num);
	}
	return (-1); // not a channel command
}

//Exceptions
Channel::Exception::Exception(const std::string& message) {
	this->_msg = message;
}

const char*	Channel::Exception::what(void) const noexcept {
	return (this->_msg.c_str());
}

//Name exceptions
Channel::NameException::NameException(const std::string& message,
std::string_view name) {
	this->_msg = message;
	this->_msg.append(" '");
	this->_msg.append(name);
	this->_msg.append("'.");
}

const char*	Channel::NameException::what(void) const noexcept {
	return (this->_msg.c_str());
}

// Exception messages
const std::string	Channel::ERR_NOT_CHANNEL_COMMAND = "Not a channel command.";
const std::string	Channel::ERR_WRONG_NUM_PARAM = "Wrong number of parameters for command.";

// NameException messages, no '.' at the end
const std::string	Channel::ERR_CREATE_CHANNEL = "Could not create a channel wth name ";
