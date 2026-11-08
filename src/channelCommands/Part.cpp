#include "Part.hpp"
#include "Channel.hpp"

Part::Part(Client& client, std::unordered_map<std::string, Channel>& channels)
: AChannelCommand(client, channels) {}

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
		std::string	partMsg = ":" + who.getNickname() + "!" + who.getUsername() +
			"@" + who.getHostname() + " PART " + std::string(channelName) +
			" :" + std::string(reason) + "\r\n";

		// Sender must already be on the channel
		if (!channel.isMember(who) && !channel.isOperator(who))
			continue;

		// Reason goes to all members and operators, including the sender
		channel.publicMessage(partMsg);

		channel.removeMember(who);
		channel.removeOperator(who);
	}
}