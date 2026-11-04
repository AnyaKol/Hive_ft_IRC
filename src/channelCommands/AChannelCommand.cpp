#include "AChannelCommand.hpp"
#include "Channel.hpp"

#include <ranges>

AChannelCommand::AChannelCommand(Client& client, std::unordered_set<Channel*>& channels)
: executor{client}, allChannels{channels} {}

/*
Finds channel with given name. Throws exception if not found.

@param	const std::set<Channel*>	channels	set of all channels on
												server.
@param	const std::string_view		channelName	name of channel to find.

@returns	Channel*	returns pointer to object of class Channel; if no
						channel was found - throws exception with
						ERR_CHANNEL_NOT_FOUND message.
*/
Channel*	AChannelCommand::findChannel(const std::unordered_set<Channel*>& channels,
	const std::string_view& channelName) {

	for (Channel* channel : channels) {
		std::string_view name = channel->getName();

		if (name == channelName)
			return (channel);
	};
	throw (Channel::NameException(Channel::ERR_CHANNEL_NOT_FOUND, channelName));
}

std::vector<std::string_view>	AChannelCommand::splitByCommas(
	const std::string_view& paramWithCommas) {

	std::vector<std::string_view>	paramVector {};
	auto range = std::views::split(paramWithCommas, ",");

	for (const auto& param : range) {
		paramVector.push_back( std::string_view(param.begin(), param.end()) );
	}
	return (paramVector);
}