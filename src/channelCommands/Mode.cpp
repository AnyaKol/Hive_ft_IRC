#include "Mode.hpp"
#include "IRC.hpp"
#include "Channel.hpp"

Mode::Mode(Client& client, std::unordered_map<std::string, Channel>& channels)
: AChannelCommand(client, channels) {}

namespace mode {
	enum allModes {
		INVITE_ONLY = static_cast<int>('i'),
		PROTECTED_TOPIC = static_cast<int>('t'),
		HAS_KEY = static_cast<int>('k'),
		HAS_USER_LIMIT = static_cast<int>('l')
	};
}

void	Mode::execute(const std::vector<std::string_view>& params) {

	// MODE <channel> [<modestring> [<mode arguments>...]]
	auto channelIt = this->AChannelCommand::allChannels.find(std::string(params[0]));
	if (channelIt == this->AChannelCommand::allChannels.end())
		return; // no such channel
	Channel& channel = channelIt->second;
	Client&	who = this->AChannelCommand::executor;

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
	if (!channel.isOperator(who))
		return;
	// Apply mode chars left to right; sign toggles '+' or '-' until next sign

	bool	adding = true;
	std::size_t	argIndex = 2; // mode arguments start at params[2]
	for (char modeChar : std::string_view(params[1])) {
		if (modeChar == '+') { adding = true; continue; }
		if (modeChar == '-') { adding = false; continue; }

		switch (static_cast<int>(modeChar)) {
			case mode::INVITE_ONLY:
				channel.setInviteOnly(adding);
				break;
			case mode::PROTECTED_TOPIC:
				channel.setTopicProtected(adding);
				break;
			case mode::HAS_KEY:
				if (adding) {
					if (argIndex < params.size()) {
						channel.setKey(std::string(params[argIndex]));
						argIndex++;
					}
				} else {
					channel.clearKey();
				}
				break;
			case mode::HAS_USER_LIMIT:
				if (adding) {
					if (argIndex < params.size()) {
						channel.setUserLimit(std::stoul(std::string(params[argIndex])));
						argIndex++;
					}
				} else {
					channel.clearUserLimit();
				}
				break;
			default:
				break; // unknown mode char
		}
	}
}