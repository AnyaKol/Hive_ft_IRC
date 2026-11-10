#include "Kick.hpp"
#include "Channel.hpp"

Kick::Kick(Client& client, std::unordered_map<std::string, Channel>& channels, Server& server)
: AChannelCommand(client, channels, server) {}

void	Kick::execute(const std::vector<std::string_view>& params) {

	if (params.size() < 2 || params.size() > 3)
		throw (Channel::Exception(Channel::ERR_WRONG_NUM_PARAM));

	// KICK <channel> <user>{,<user>} [<comment>]
	std::string_view	channelName = params[0];
	std::vector<std::string_view>	nicknames = AChannelCommand::splitByCommas(params[1]);
	std::string	reason = std::string(this->AChannelCommand::executor.getNickname());
	if (params.size() == 3)
		reason = std::string(params[2]);

	auto channelIt = this->AChannelCommand::allChannels.find(std::string(channelName));
	if (channelIt == this->AChannelCommand::allChannels.end()) {
		Channel::sendNumeric(this->AChannelCommand::executor, IRC::ERR_NOSUCHCHANNEL,
			std::string(channelName), "No such channel");
		return;
	}
	Channel& channel = channelIt->second;

	// Only operators may kick
	Client&	who = this->AChannelCommand::executor;
	if (!channel.isOperator(who)) {
		Channel::sendNumeric(who, IRC::ERR_CHANOPRIVSNEEDED,
			std::string(channelName), "You're not channel operator");
		return;
	}

	// Kick each target: announce, then remove from members and operators
	for (std::string_view nickname : nicknames) {
		Client* target = NULL;
		for (Client* member : channel.getMembers()) {
			if (member->getNickname() == nickname) {
				target = member;
				break;
			}
		}
		for (Client* op : channel.getOperators()) {
			if (op->getNickname() == nickname) {
				target = op;
				break;
			}
		}

		if (!target) {
			// 441: target nick is not on this channel
			Channel::sendNumeric(who, IRC::ERR_USERNOTINCHANNEL,
				std::string(nickname), std::string(channelName));
			continue;
		}

		std::string	kickMsg = ":" + who.getNickname() + "!" + who.getUsername() +
			"@" + who.getHostname() + " KICK " + std::string(channelName) + " " +
			std::string(nickname) + " :" + reason + "\r\n";
		channel.publicMessage(kickMsg);

		channel.removeMember(*target);
		channel.removeOperator(*target);
	}
}