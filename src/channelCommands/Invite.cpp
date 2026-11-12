#include "Invite.hpp"
#include "Channel.hpp"
#include "Server.hpp"

Invite::Invite(Client& client, std::unordered_map<std::string, Channel>& channels, Server& server)
: AChannelCommand(client, channels, server) {}

void	Invite::execute(const std::vector<std::string_view>& params) {

	if (params.size() != 2)
		throw (Channel::Exception(Channel::ERR_WRONG_NUM_PARAM));

	// INVITE <nickname> <channel>
	std::string_view	nickname = params[0];

	auto channelIt = this->AChannelCommand::allChannels.find(std::string(params[1]));
	if (channelIt == this->AChannelCommand::allChannels.end()) {
		Channel::sendNumeric(this->AChannelCommand::executor, IRC::ERR_NOSUCHCHANNEL,
			std::string(params[1]), "No such channel");
		return;
	}
	Channel& channel = channelIt->second;

	// Only operators may invite
	Client&	who = this->AChannelCommand::executor;
	if (!channel.isOperator(who)) {
		Channel::sendNumeric(who, IRC::ERR_CHANOPRIVSNEEDED,
			std::string(params[1]), "You're not channel operator");
		return;
	}

	// Target must exist on the server
	Client* target = this->server.getClientByNick(std::string(nickname));
	if (!target) {
		Channel::sendNumeric(who, IRC::ERR_NOSUCHNICK, std::string(nickname), "No such nick/channel");
		return;
	}

	// Reject if target is already on the channel
	for (Client* member : channel.getMembers())
		if (member->getNickname() == nickname) {
			Channel::sendNumeric(who, IRC::ERR_USERONCHANNEL,
				std::string(nickname), std::string(params[1]));
			return;
		}
	for (Client* op : channel.getOperators())
		if (op->getNickname() == nickname) {
			Channel::sendNumeric(who, IRC::ERR_USERONCHANNEL,
				std::string(nickname), std::string(params[1]));
			return;
		}

	if (!channel.invite(std::string(nickname)))
		return; // already invited

	// 341 RPL_INVITING: confirm to the inviter
	Channel::sendNumeric(who, IRC::RPL_INVITING, std::string(nickname), std::string(params[1]));

	// INVITE message, with the issuer as <source>, to the target user
	std::string	invMsg = ":" + this->AChannelCommand::executor.getNickname() + "!" +
		this->AChannelCommand::executor.getUsername() + "@" +
		this->AChannelCommand::executor.getHostname() + " INVITE " + target->getNickname() + " " +
		std::string(channel.getName()) + "\r\n";
	target->appendToWriteBuffer(invMsg);
}