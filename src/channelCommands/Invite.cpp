#include "Invite.hpp"
#include "Channel.hpp"

Invite::Invite(Client& client, std::unordered_map<std::string, Channel>& channels)
: AChannelCommand(client, channels) {}

void	Invite::execute(const std::vector<std::string_view>& params) {

	if (params.size() != 2)
		throw (Channel::Exception(Channel::ERR_WRONG_NUM_PARAM));

	// INVITE <nickname> <channel>
	std::string_view	nickname = params[0];

	auto channelIt = this->AChannelCommand::allChannels.find(std::string(params[1]));
	if (channelIt == this->AChannelCommand::allChannels.end())
		return; // no such channel
	Channel& channel = channelIt->second;

	// Only operators may invite
	Client&	who = this->AChannelCommand::executor;
	if (!channel.isOperator(who))
		return;

	// Reject if target is already on the channel
	for (Client* member : channel.getMembers())
		if (member->getNickname() == nickname)
			return;
	for (Client* op : channel.getOperators())
		if (op->getNickname() == nickname)
			return;

	if (!channel.invite(std::string(nickname)))
		return; // already invited
}