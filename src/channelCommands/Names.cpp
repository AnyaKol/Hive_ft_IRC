#include "Names.hpp"
#include "Channel.hpp"

Names::Names(Client& client, std::unordered_map<std::string, Channel>& channels)
: AChannelCommand(client, channels) {}

void	Names::execute(const std::vector<std::string_view>& params) {

	Client& mutableClient = const_cast<Client&>(this->AChannelCommand::executor);
	std::string nick = mutableClient.getNickname();

	// Check for required parameters
	if (params.empty()) {
		mutableClient.appendToWriteBuffer("461 " + nick + " NAMES :Not enough parameters\r\n");
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
		mutableClient.appendToWriteBuffer("366 " + nick + " " + targetName + " :End of /NAMES list\r\n");
		return;
	}

	std::string nameList = "";

	// Append operators with '@' prefix
	for (const Client* op : targetChannel->getOperators()) {
		if (!nameList.empty())
			nameList += " ";
		nameList += "@" + op->getNickname();
	}

	// Append regular channel members
	for (const Client* member : targetChannel->getMembers()) {
		if (!nameList.empty())
			nameList += " ";
		nameList += member->getNickname();
	}

	// Send name reply (353) and end of names list (366)
	mutableClient.appendToWriteBuffer("353 " + nick + " = " + targetName + " :" + nameList + "\r\n");
	mutableClient.appendToWriteBuffer("366 " + nick + " " + targetName + " :End of /NAMES list\r\n");
}