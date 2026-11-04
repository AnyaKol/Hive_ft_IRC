#pragma once

#include "Client.hpp"

#include <vector>
#include <string_view>
#include <unordered_set>

class Channel;

class AChannelCommand {
	protected:
		Channel*	findChannel(const std::unordered_set<Channel*>& channels,
			const std::string_view& channelName);
		
		std::vector<std::string_view>	splitByCommas(
			const std::string_view& paramWithCommas);

	public:
		AChannelCommand(void) = delete;
		AChannelCommand(Client& client, std::unordered_set<Channel*>& channels);
		virtual ~AChannelCommand(void) = default;

		virtual void	execute(const std::vector<std::string_view>& params) = 0;
		
		Client&							executor;
		std::unordered_set<Channel*>&	allChannels;
};