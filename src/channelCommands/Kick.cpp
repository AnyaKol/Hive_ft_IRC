#include "Kick.hpp"
#include "Channel.hpp"

#include <iostream>

Kick::Kick(Client& client, std::set<Channel>& channels)
: AChannelCommand(client, channels) {}

void	Kick::execute(const std::vector<std::string_view>& params) {
	(void) params;

	std::cout << "Kick command message" << std::endl;
}