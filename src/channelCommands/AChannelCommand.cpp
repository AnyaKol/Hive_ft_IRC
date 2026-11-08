#include "AChannelCommand.hpp"
#include "Channel.hpp"

#include <ranges>

/*
Finds channel with given name or creates new channel if no channel was found.
Throws exception if error occured (if channel with this name exists).

@param	const std::string_view		channelName	name of channel to find.

@returns	Channel&	finds or creates an object of class Channel in the
						server's unordered_map<std::string, Channel> and
						returns a reference to it; if error occured - throws
						exception with ERR_CREATE_CHANNEL message.
*/
Channel&	AChannelCommand::findChannel(const std::string_view& channelName) {

	auto it = this->allChannels.find(std::string(channelName));
	if (it != this->allChannels.end())
		return (it->second);

	//If no channel found - create new channel;
	auto inserted = this->allChannels.emplace(
		std::string(channelName), Channel(channelName, this->executor)
	);
	if (!inserted.second)
		throw (Channel::NameException(Channel::ERR_CREATE_CHANNEL, channelName));
	return (inserted.first->second);
}

std::vector<std::string_view>	AChannelCommand::splitByCommas( 
	const std::string_view& paramWithCommas) {

	std::vector<std::string_view>	paramVector {};
	auto range = std::ranges::views::split(paramWithCommas, ",");

	for (const auto& param : range) {
		paramVector.push_back( std::string_view(param.begin(), param.end()) );
	}
	return (paramVector);
}