#include "Channel.hpp"

//Commands
#include "AChannelCommand.hpp"
#include "Invite.hpp"
#include "Join.hpp"
#include "Kick.hpp"
#include "Mode.hpp"
#include "Names.hpp"
#include "Part.hpp"
#include "Topic.hpp"

#include "Client.hpp"
#include "IRC.hpp"

#include <algorithm>

namespace mode {
	enum allModes {
		INVITE_ONLY = static_cast<int>('i'),
		PROTECTED_TOPIC = static_cast<int>('t'),
		HAS_KEY = static_cast<int>('k'),
		HAS_USER_LIMIT = static_cast<int>('l')
	};
}

// Set user limit to 0 instead of NAN because NAN is float
Channel::Channel(std::string_view name, Client& creator) {
	this->_name = name;
	this->_topic = "";
	this->_key = "";
	this->_userLimit = 0;
	this->addOperator(creator);
}

//Getters
std::string_view	Channel::getName(void) const {
	return( this->_name );
}

std::string_view	Channel::getTopic(void) const {
	return( this->_topic );
}

std::size_t Channel::getUserLimit(void) const {
	return( this->_userLimit );
}

//Setters
void	Channel::setName(std::string_view name) {
	if (name.empty()) {
		//print error
		return;
	}
	else
		this->_name = name;
}

void	Channel::setTopic(std::string_view topic, Client& client) {
	if (this->_protectedTopic) {
		if ( !this->_operators.contains(&client) ) {
			//print error
			return;
		}
	}
	this->_topic = topic;
}

void	Channel::setUserLimit(std::size_t limit) {
	if (!this->_hasUserLimit) {
		//print error
		return;
	}
	if (this->_members.size() + this->_operators.size() > limit) {
		//print error
		return;
	}
	else
		this->_userLimit = limit;
}

void	Channel::addMember(Client& client) {
	std::pair<iterator, bool>	result;

	if (this->isFull()) {
		//print error
		return;
	}
	result = this->_members.insert(&client);
	if (!result.second) {
		//print error Client already joined
		return;
	}
}

void	Channel::addOperator(Client& client) {
	std::pair<iterator, bool>	result;

	if (this->isFull()) {
		//print error
		return;
	}
	result = this->_operators.insert(&client);
	if (!result.second) {
		//print error Client already joined
		return;
	}
}

void	Channel::removeMember(Client& client) {
	this->_members.erase( this->_members.find(&client) );
}

void	Channel::removeOperator(Client& client) {
	this->_operators.erase( this->_operators.find(&client) );
}

void	Channel::memberToOperator(Client& client) {
	this->removeMember(client);
	this->addOperator(client);
}

void	Channel::operatorToMember(Client& client) {
	this->removeOperator(client);
	this->addMember(client);
}

/* Channel modes:
 *  i - _inviteOnly
 *  t - _protectedTopic
 *  k - _hasKey
 *  o - memberToOperator / operatorToMember in MODE
 *  l - _hasUserLimit
 */
void	Channel::changeMode(char mode, bool value) {
	switch (static_cast<int>(mode)) {
		case mode::INVITE_ONLY:
			this->_inviteOnly = value;
			break;
		case mode::PROTECTED_TOPIC:
			this->_protectedTopic = value;
			break;
		case mode::HAS_KEY:
			this->_hasKey = value;
			break;
		case mode::HAS_USER_LIMIT:
			this->_hasUserLimit = value;
			break;
		default:
			// print error
			break;
	}
}

bool	Channel::isFull(void) const {
	if (!this->_hasUserLimit)
		return (false);

	if (this->_members.size() + this->_operators.size() == this->_userLimit)
		return (true);
	return (false);
}

bool	isValidChannelName(std::string_view name) {
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

void	Channel::publicMessage(const std::string& msg) const {
	if (msg.size() == 0)
		return;

	std::for_each(this->_operators.begin(), this->_operators.end(), [&](Client* client) {
		(*client).appendToReadBuffer(msg);
	});
	std::for_each(this->_members.begin(), this->_members.end(), [&](Client* client) {
		(*client).appendToReadBuffer(msg);
	});
}

// Commands
bool	Channel::isChannelCommand(Client& client, std::string& command) {

	try {
		AChannelCommand cmd = Channel::makeCommand(command);
		cmd.execute(client);
	} catch (NotChannelCommandException &e) {
		return (false);
	}
	return (true);
}

AChannelCommand&	Channel::makeCommand(std::string& command) {
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
			return (Invite());
		case allCommands::JOIN:
			return (Join());
		case allCommands::KICK:
			return (Kick());
		case allCommands::MODE:
			return (Mode());
		case allCommands::NAMES:
			return (Names());
		case allCommands::PART:
			return (Part());
		case allCommands::TOPIC:
			return (Topic());
		default:
			throw (NotChannelCommandException());
	}
}

//TODO: Optimize for loop
int	Channel::makeCommandNumber(std::string& command) {
	std::string	commandStrings[1] = {
		"JOIN"
	};
	int commandNumber = 0;

	for (str : commandStrings) {
		if (command == commandStrings[commandNumber])
			break ;
	}
	return (commandNumber);
}