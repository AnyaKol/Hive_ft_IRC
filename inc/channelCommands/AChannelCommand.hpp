#pragma once

#include "Client.hpp"

#include <vector>
#include <string_view>
#include <string>
#include <unordered_map>

class Channel;

class AChannelCommand {
	protected:
		Channel&	findChannel(const std::string_view& channelName);
		
		std::vector<std::string_view>	splitByCommas(
			const std::string_view& paramWithCommas);

	public:
		AChannelCommand(void) = delete;
		AChannelCommand(Client& client, std::unordered_map<std::string, Channel>& channels) : executor(client), allChannels(channels) {};
		virtual ~AChannelCommand(void) = default;

		typedef std::unordered_map<std::string, Channel>::iterator	iterator;

		virtual void	execute(const std::vector<std::string_view>& params) = 0;
		
		Client&				executor;
		std::unordered_map<std::string, Channel>&	allChannels;
};