#include "Topic.hpp"
#include "Channel.hpp"

Topic::Topic(Client& client, std::unordered_map<std::string, Channel>& channels, Server& server)
: AChannelCommand(client, channels, server) {}

void	Topic::execute(const std::vector<std::string_view>& params) {

	Client& mutableClient = const_cast<Client&>(this->AChannelCommand::executor);
	std::string nick = mutableClient.getNickname();

	// Check for required parameters
	if (params.empty()) {
		Channel::sendNumeric(mutableClient, IRC::ERR_NEEDMOREPARAMS, "TOPIC", "Not enough parameters");
		return;
	}

	std::string targetName(params[0]);
	Channel* targetChannel = nullptr;

	// Search for the target channel in the server's channels map
	auto channelIt = this->AChannelCommand::allChannels.find(targetName);
	if (channelIt != this->AChannelCommand::allChannels.end())
		targetChannel = &channelIt->second;

	// If channel does not exist, return error
	if (!targetChannel) {
		Channel::sendNumeric(mutableClient, IRC::ERR_NOSUCHCHANNEL, targetName, "No such channel");
		return;
	}

	// Verify if the client is a member or operator of the channel
	bool isOp = targetChannel->isOperator(mutableClient);
	bool isMem = targetChannel->isMember(mutableClient);

	if (!isOp && !isMem) {
		Channel::sendNumeric(mutableClient, IRC::ERR_NOTONCHANNEL, targetName, "You're not on that channel");
		return;
	}

	// If only 1 parameter is provided, read the current topic
	if (params.size() == 1) {
		std::string currentTopic(targetChannel->getTopic());
		if (currentTopic.empty()) {
			Channel::sendNumeric(mutableClient, IRC::RPL_NOTOPIC, targetName, "No topic is set");
		} else {
			Channel::sendNumeric(mutableClient, IRC::RPL_TOPIC, targetName, currentTopic);
		}
		return;
	}

	// If setting a topic, check if topic is protected and client is not an operator
	if (targetChannel->isTopicProtected() && !isOp) {
		Channel::sendNumeric(mutableClient, IRC::ERR_CHANOPRIVSNEEDED, targetName, "You're not channel operator");
		return;
	}

	// Update topic and broadcast change to the channel
	std::string_view newTopic = params[1];
	targetChannel->setTopic(newTopic, mutableClient);

	std::string broadcastMsg = ":" + nick + " TOPIC " + targetName + " :" + std::string(newTopic) + "\r\n";
	targetChannel->publicMessage(broadcastMsg);
}