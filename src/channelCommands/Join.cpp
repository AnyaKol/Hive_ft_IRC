#include "Join.hpp"
#include "Channel.hpp"

Join::Join(Client& client, std::unordered_map<std::string, Channel>& channels, Server& server)
: AChannelCommand(client, channels, server) {}

 // Servers may validate the <key> value, eg. to forbid spaces; can forbid a <key> to start from channel name prefix ('#' or '&')
/*
	JOIN #channel1,#channel2,#channel3 key2,key3
	After parsing:
	[#channel1,#channel2,#channel3] [key2,key3,x]
*/
void	Join::execute(const std::vector<std::string_view>& params) {

	//Check if no parameters given
	if (params.size() != 1 && params.size() != 2)
		throw (Channel::Exception(Channel::ERR_WRONG_NUM_PARAM));

	// JOIN <channel>{,<channel>} [<key>{,<key>}]
	// Split <channels> and <keys>
	this->channels = AChannelCommand::splitByCommas(params[0]);
	if (params.size() == 2)
		this->keys = AChannelCommand::splitByCommas(params[1]);


	// Go through <channels> and add executing client; if <key> = "x" then skip it
	// Reject empty/invalid channel names (irssi sends "JOIN :" with empty trailing on connect)
	for (std::string_view channelName : this->channels) {
		if (!Channel::isValidChannelName(channelName))
			return;
	}

	try {
		if (this->keys.size() > 0) {
			for (std::string_view channelName : this->channels) {
				bool	channelFound = false;
				Channel& channel = findChannel(channelName, channelFound);
				std::vector<std::string_view>::iterator nextKey = this->keys.begin();

				// Creator of a new channel is already operator; just confirm the JOIN to them
				if (channelFound) {
					if (*nextKey != "x")
						channel.addMember(this->AChannelCommand::executor, *nextKey);
					else
						channel.addMember(this->AChannelCommand::executor);
					if (!channel.isMember(this->AChannelCommand::executor))
						continue;
				}
				std::string	joinMsg = ":" + this->AChannelCommand::executor.getNickname() + "!" +
					this->AChannelCommand::executor.getUsername() + "@" +
					this->AChannelCommand::executor.getHostname() + " JOIN " +
					std::string(channelName) + "\r\n";
				channel.publicMessage(joinMsg);
				// Standard JOIN reply: tell the joiner who is on the channel
				std::vector<std::string>	opNicks, memNicks;
				for (const Client* op : channel.getOperators())
					opNicks.push_back(op->getNickname());
				for (const Client* member : channel.getMembers())
					memNicks.push_back(member->getNickname());
				Channel::sendNumeric(this->AChannelCommand::executor, IRC::RPL_NAMREPLY,
					std::string(channelName), IRC::Reply::namesList(opNicks, memNicks));
				Channel::sendNumeric(this->AChannelCommand::executor, IRC::RPL_ENDOFNAMES,
					std::string(channelName), "End of /NAMES list");

				if (nextKey < this->keys.end())
					nextKey++;
			}
		} else {
			for (std::string_view channelName : this->channels) {
				bool	channelFound = false;
				Channel& channel = findChannel(channelName, channelFound);

				// Creator of a new channel is already operator; just confirm the JOIN to them
				if (channelFound) {
					channel.addMember(this->AChannelCommand::executor);
					if (!channel.isMember(this->AChannelCommand::executor))
						continue;
				}
				std::string	joinMsg = ":" + this->AChannelCommand::executor.getNickname() + "!" +
					this->AChannelCommand::executor.getUsername() + "@" +
					this->AChannelCommand::executor.getHostname() + " JOIN " +
					std::string(channelName) + "\r\n";
				channel.publicMessage(joinMsg);
				// Standard JOIN reply: tell the joiner who is on the channel
				std::vector<std::string>	opNicks, memNicks;
				for (const Client* op : channel.getOperators())
					opNicks.push_back(op->getNickname());
				for (const Client* member : channel.getMembers())
					memNicks.push_back(member->getNickname());
				Channel::sendNumeric(this->AChannelCommand::executor, IRC::RPL_NAMREPLY,
					std::string(channelName), IRC::Reply::namesList(opNicks, memNicks));
				Channel::sendNumeric(this->AChannelCommand::executor, IRC::RPL_ENDOFNAMES,
					std::string(channelName), "End of /NAMES list");
			}
		}
	} catch (Channel::Exception &e) { // Expected command failures
		return;
	} catch (Channel::NameException &e) {
		return;
	}
}