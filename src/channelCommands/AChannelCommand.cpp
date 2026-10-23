#include "AChannelCommand.hpp"
#include "Channel.hpp"

/*
Finds channel with given name. Throws exception if not found.

@param	const std::set<Channel*>	channels	set of all channels on
												server.
@param	const std::string_view		channelName	name of channel to find.

@returns	const Channel*  returns pointer to object of class Channel; if no
							channel was found - throws exception with
							ERR_CHANNEL_NOT_FOUND message.
*/
const Channel*	AChannelCommand::findChannel(const std::unordered_set<Channel*>& channels,
	const std::string_view& channelName) {
	for (Channel* channel : channels) {
		std::string_view name = channel->getName();

		if (name == channelName)
			return (channel);
	};
	throw (Channel::NameException(Channel::ERR_CHANNEL_NOT_FOUND, channelName));
}