#include "Topic.hpp"
#include "Channel.hpp"
#include "Client.hpp"

void	Topic::execute(std::unordered_set<Channel*>& channels,
	const Client& client, const std::vector<std::string_view>& params) {

	Client& mutableClient = const_cast<Client&>(client);
	std::string nick = mutableClient.getNickname();

	// Check for required parameters
	if (params.empty()) {
		mutableClient.appendToWriteBuffer("461 " + nick + " TOPIC :Not enough parameters\r\n");
		return;
	}

	std::string targetName(params[0]);
	Channel* targetChannel = nullptr;

	// Search for the target channel in the server's channels set
	for (Channel* chan : channels) {
		if (chan->getName() == targetName) {
			targetChannel = chan;
			break;
		}
	}

	// If channel does not exist, return error
	if (!targetChannel) {
		mutableClient.appendToWriteBuffer("403 " + nick + " " + targetName + " :No such channel\r\n");
		return;
	}

	// Verify if the client is a member or operator of the channel
	bool isOp = targetChannel->getOperators().contains(&mutableClient);
	bool isMem = targetChannel->getMembers().contains(&mutableClient);

	if (!isOp && !isMem) {
		mutableClient.appendToWriteBuffer("442 " + nick + " " + targetName + " :You're not on that channel\r\n");
		return;
	}

	// If only 1 parameter is provided, read the current topic
	if (params.size() == 1) {
		std::string currentTopic(targetChannel->getTopic());
		if (currentTopic.empty()) {
			mutableClient.appendToWriteBuffer("331 " + nick + " " + targetName + " :No topic is set\r\n");
		} else {
			mutableClient.appendToWriteBuffer("332 " + nick + " " + targetName + " :" + currentTopic + "\r\n");
		}
		return;
	}

	// If setting a topic, check if topic is protected and client is not an operator
	if (targetChannel->isTopicProtected() && !isOp) {
		mutableClient.appendToWriteBuffer("482 " + nick + " " + targetName + " :You're not channel operator\r\n");
		return;
	}

	// Update topic and broadcast change to the channel
	std::string_view newTopic = params[1];
	targetChannel->setTopic(newTopic, mutableClient);

	std::string broadcastMsg = ":" + nick + " TOPIC " + targetName + " :" + std::string(newTopic) + "\r\n";
	targetChannel->publicMessage(broadcastMsg);
}