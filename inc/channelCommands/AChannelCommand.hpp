#pragma once

#include "Client.hpp"

#include <vector>
#include <string_view>
#include <set>

class Channel;

class AChannelCommand {
	protected:
		Channel&	findChannel(const std::string_view& channelName);
		
		std::vector<std::string_view>	splitByCommas(
			const std::string_view& paramWithCommas);

	public:
		AChannelCommand(void) = delete;
		AChannelCommand(Client& client, std::set<Channel>& channels) : executor(client), allChannels(channels) {};
		virtual ~AChannelCommand(void) = default;

		typedef std::set<Channel>::iterator	iterator;

		virtual void	execute(const std::vector<std::string_view>& params) = 0;
		
		Client&				executor;
		std::set<Channel>&	allChannels;
};