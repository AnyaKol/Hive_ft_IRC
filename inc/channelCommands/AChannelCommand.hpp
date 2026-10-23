#pragma once

#include "Client.hpp"
#include <vector>
#include <string_view>
#include <set>

class Channel;

class AChannelCommand {
	protected:
		const Channel*	findChannel(const std::set<Channel*> channels,
			const std::string_view channelName);

	public:
		virtual ~AChannelCommand(void) = default;

		virtual void	execute(const Client& client,
			const std::vector<std::string_view>& params) = 0;
};