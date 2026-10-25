#pragma once

#include "Client.hpp"

#include <vector>
#include <string_view>
#include <unordered_set>

class Channel;

class AChannelCommand {
	protected:
		const Channel*	findChannel(const std::unordered_set<Channel*>& channels,
			const std::string_view& channelName);

	public:
		virtual ~AChannelCommand(void) = default;

		virtual void	execute(std::unordered_set<Channel*>& channels,
			const Client& client, const std::vector<std::string_view>& params) = 0;
};