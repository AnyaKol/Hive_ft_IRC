#include "Names.hpp"
#include "Channel.hpp"

Names::Names(Client& client, std::unordered_map<std::string, Channel>& channels, Server& server)
: AChannelCommand(client, channels, server) {}

void	Names::execute(const std::vector<std::string_view>& params) {

	Client& mutableClient = const_cast<Client&>(this->AChannelCommand::executor);
	std::string nick = mutableClient.getNickname();

	// Check for required parameters
	if (params.empty()) {
		Channel::sendNumeric(mutableClient, IRC::ERR_NEEDMOREPARAMS, "NAMES", "Not enough parameters");
		return;
	}

	std::string targetName(params[0]);
	const Channel* targetChannel = nullptr;

	// Search for the target channel in the server's channels map
	auto channelIt = this->AChannelCommand::allChannels.find(targetName);
	if (channelIt != this->AChannelCommand::allChannels.end())
		targetChannel = &channelIt->second;

	// If channel does not exist, IRC protocol specifies sending end of NAMES
	if (!targetChannel) {
		Channel::sendNumeric(mutableClient, IRC::RPL_ENDOFNAMES, targetName, "End of /NAMES list");
		return;
	}

	// Send name reply (353) and end of names list (366)
	std::vector<std::string>	opNicks, memNicks;
	for (const Client* op : targetChannel->getOperators())
		opNicks.push_back(op->getNickname());
	for (const Client* member : targetChannel->getMembers())
		memNicks.push_back(member->getNickname());
	Channel::sendNumeric(mutableClient, IRC::RPL_NAMREPLY,
		targetName, IRC::Reply::namesList(opNicks, memNicks));
	Channel::sendNumeric(mutableClient, IRC::RPL_ENDOFNAMES, targetName, "End of /NAMES list");
}