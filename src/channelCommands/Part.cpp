#include "Part.hpp"
#include "Channel.hpp"

Part::Part(Client& client, std::unordered_map<std::string, Channel>& channels, Server& server)
: AChannelCommand(client, channels, server) {}

void	Part::execute(const std::vector<std::string_view>& params) {

	if (params.empty() || params.size() > 2)
		throw (Channel::Exception(Channel::ERR_WRONG_NUM_PARAM));

	// Split <channels>; reason defaults to "Leaving" when not provided
	std::vector<std::string_view>	channelNames = AChannelCommand::splitByCommas(params[0]);
	std::string_view	reason = "Leaving";
	if (params.size() == 2)
		reason = params[1];

	Client&	who = this->AChannelCommand::executor;

	for (std::string_view channelName : channelNames) {
		auto it = this->AChannelCommand::allChannels.find(std::string(channelName));
		if (it == this->AChannelCommand::allChannels.end())
			continue; // no such channel
		Channel& channel = it->second;

		// Sender must already be on the channel
		if (!channel.isMember(who) && !channel.isOperator(who))
			continue;

		// Create custom user message
		std::string	partMsg = ":" + who.getNickname() + "!" + who.getUsername() +
			"@" + who.getHostname() + " PART " + std::string(channelName) +
			" :" + std::string(reason) + "\r\n";
		// Sender also gets the message
		channel.publicMessage(partMsg);

		channel.removeMember(who);
		channel.removeOperator(who);

		// Channel with nobody left is destroyed (state like topic/modes/key is lost)
		if (channel.getMembers().empty() && channel.getOperators().empty())
			this->AChannelCommand::allChannels.erase(it);
	}
}