#include "AChannelCommand.hpp"
#include "Channel.hpp"

#include <ranges>

/*
Finds channel with given name or creates new channel if no channel was found.
Throws exception if error occured (if channel with this name exists).

@param	const std::string_view		channelName	name of channel to find.

@returns	std::unique_ptr<Channel>&	finds or creates a new object of class
			Channel and returns a pointer to it; if error occured - throws
			exception with ERR_CREATE_CHANNEL message.
*/
Channel&	AChannelCommand::findChannel(const std::string_view& channelName) {

	for (const Channel& channel : this->allChannels) {
		std::string_view name = channel.getName();

		if (name == channelName)
			return ( const_cast<Channel&>(channel) );
	};

	//If no channel found - create new channel;
	std::pair<iterator,bool> it = this->allChannels.insert(
		Channel(channelName, this->executor)
	);
	if (!it.second)
		throw (Channel::NameException(Channel::ERR_CREATE_CHANNEL, channelName));
	return ( const_cast<Channel&>(*(it.first)) );
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