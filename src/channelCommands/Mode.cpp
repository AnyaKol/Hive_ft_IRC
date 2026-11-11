#include "Mode.hpp"
#include "IRC.hpp"
#include "Channel.hpp"

Mode::Mode(Client& client, std::unordered_map<std::string, Channel>& channels, Server& server)
: AChannelCommand(client, channels, server) {}

namespace mode {
	enum allModes {
		INVITE_ONLY = static_cast<int>('i'),
		PROTECTED_TOPIC = static_cast<int>('t'),
		HAS_KEY = static_cast<int>('k'),
		HAS_USER_LIMIT = static_cast<int>('l'),
		OPS_PRIVILIGES = static_cast<int>('o')
	};
}

/* Records one applied mode change for the broadcast:
 * appends the mode char to appliedModes (with sign when it flips),
 * and its argument (if any) to appliedArgs. */
static void	record(char sign, char modeChar, std::string& appliedModes,
	std::vector<std::string>& appliedArgs, const std::string& arg)
{
	char	last = appliedModes.empty() ? 0 : appliedModes[appliedModes.size() - 1];
	if (last != '+' && last != '-')
		appliedModes += sign;
	appliedModes += modeChar;
	appliedArgs.push_back(arg);
}

void	Mode::execute(const std::vector<std::string_view>& params) {

	// MODE <target> [<modestring> [<mode arguments>...]]
	Client&	who = this->AChannelCommand::executor;

	// Silently ignore MODE on a nickname
	if (params[0][0] != '#' && params[0][0] != '&')
		return;

	auto channelIt = this->AChannelCommand::allChannels.find(std::string(params[0]));
	if (channelIt == this->AChannelCommand::allChannels.end()) {
		Channel::sendNumeric(who, IRC::ERR_NOSUCHCHANNEL, std::string(params[0]), "No such channel");
		return;
	}
	Channel& channel = channelIt->second;

	// Only 1 argument: query - reply with current modes, key hidden
	if (params.size() == 1) {
		std::string	modes = "+";
		std::string	modeArgs = "";
		if (channel.isInviteOnly()) modes += "i";
		if (channel.isTopicProtected()) modes += "t";
		if (channel.hasKey()) { modes += "k"; modeArgs += " *"; }
		if (channel.hasUserLimit()) { modes += "l"; modeArgs += " " + std::to_string(channel.getUserLimit()); }

		std::string	reply = ":" + std::string(IRC::SERVER_NAME) + " 324 " + who.getNickname() +
			" " + std::string(params[0]) + " " + modes + modeArgs + "\r\n";
		who.appendToWriteBuffer(reply);
		return;
	}

	// Only operators may change modes
	if (!channel.isOperator(who)) {
		Channel::sendNumeric(who, IRC::ERR_CHANOPRIVSNEEDED,
			std::string(params[0]), "You're not channel operator");
		return;
	}

	// Apply mode chars left to right; sign toggles '+' or '-' until next sign.
	// Applied changes are recorded so they can be broadcast to the channel.
	bool			adding = true;
	std::size_t		argIndex = 2; // mode arguments start at params[2]
	std::string		appliedModes; // modestring actually applied
	std::vector<std::string>	appliedArgs;  // args matching applied mode chars

	for (char modeChar : std::string_view(params[1])) {
		if (modeChar == '+') { adding = true; continue; }
		if (modeChar == '-') { adding = false; continue; }

		char	sign = adding ? '+' : '-';

		switch (static_cast<int>(modeChar)) {
			case mode::INVITE_ONLY:
				channel.setInviteOnly(adding);
				record(sign, modeChar, appliedModes, appliedArgs, "");
				break;
			case mode::PROTECTED_TOPIC:
				channel.setTopicProtected(adding);
				record(sign, modeChar, appliedModes, appliedArgs, "");
				break;
			case mode::HAS_KEY:
				if (adding) {
					if (argIndex < params.size()) {
						channel.setKey(std::string(params[argIndex]), who);
						record(sign, modeChar, appliedModes, appliedArgs, std::string(params[argIndex]));
						argIndex++;
					}
				} else {
					channel.clearKey();
					record(sign, modeChar, appliedModes, appliedArgs, "");
				}
				break;
			case mode::HAS_USER_LIMIT:
				if (adding) {
					if (argIndex < params.size()) {
						// Accept only digits: rejects signs, spaces, and empty input
						std::string_view	limitStr = params[argIndex];

						if (limitStr.empty()
							|| limitStr.size() > 19	// cannot overflow size_t (max 20 digits)
							|| limitStr.find_first_not_of("0123456789") != std::string_view::npos) {
							who.appendToWriteBuffer(IRC::Reply::errInvalidModeParam(who.getNickname(),
								std::string(params[0]), 'l', limitStr, "Invalid limit parameter"));
						} else {
							channel.setUserLimit(std::stoul(std::string(limitStr)));
							record(sign, modeChar, appliedModes, appliedArgs, std::string(limitStr));
						}
						argIndex++;
					}
				} else {
					channel.clearUserLimit();
					record(sign, modeChar, appliedModes, appliedArgs, "");
				}
				break;
			case mode::OPS_PRIVILIGES:
				if (argIndex < params.size()) {
					std::string_view	nickname = params[argIndex];
					Client*				target = channel.getClientByNick(std::string(nickname));

					if (!target) {
						Channel::sendNumeric(who, IRC::ERR_NOSUCHNICK, std::string(nickname), "No such nick/channel");
						return;
					}

					if (adding) {
						channel.memberToOperator(*target);
					} else {
						channel.operatorToMember(*target);
					}
					record(sign, modeChar, appliedModes, appliedArgs, std::string(nickname));
					argIndex++;
				}
				break;
			default:
				// 472 ERR_UNKNOWNMODE: char exists in the modestring but server
				// does not implement it. '+'/'-' were consumed above.
				Channel::sendNumeric(who, IRC::ERR_UNKNOWNMODE,
					std::string(params[0]), std::string(1, modeChar));
				break;
		}
	}

	// Broadcast applied changes to the whole channel; executor gets the message
	// irssi updates cached nicklist from this message
	if (!appliedModes.empty()) {
		std::string	modeMsg = ":" + who.getNickname() + "!" + who.getUsername() + "@" +
			who.getHostname() + " MODE " + std::string(params[0]) + " " + appliedModes;
		for (const std::string& arg : appliedArgs) {
			if (!arg.empty())
				modeMsg += " " + arg;
		}
		modeMsg += "\r\n";
		channel.publicMessage(modeMsg); // no sender exclusion: RFC 2812 3.2.3
	}
}