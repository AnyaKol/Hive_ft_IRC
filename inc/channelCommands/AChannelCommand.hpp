#pragma once

#include "Client.hpp"

#include <vector>
#include <string_view>
#include <string>
#include <unordered_map>

class Channel;
class Server;

class AChannelCommand {
	protected:
		Channel&	findChannel(const std::string_view& channelName, bool& channelFound);

		std::vector<std::string_view>	splitByCommas(
			const std::string_view& paramWithCommas);

	public:
		AChannelCommand(void) = delete;
		AChannelCommand(Client& client, std::unordered_map<std::string, Channel>& channels,
			Server& server) : executor(client), allChannels(channels), server(server) {};
		virtual ~AChannelCommand(void) = default;

		typedef std::unordered_map<std::string, Channel>::iterator	iterator;

		virtual void	execute(const std::vector<std::string_view>& params) = 0;

		Client&				executor;
		std::unordered_map<std::string, Channel>&	allChannels;
		Server&				server;
};