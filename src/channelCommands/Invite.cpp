#include "Invite.hpp"
#include "Channel.hpp"

#include <iostream>

Invite::Invite(Client& client, std::unordered_map<std::string, Channel>& channels)
: AChannelCommand(client, channels) {}

void	Invite::execute(const std::vector<std::string_view>& params) {
	(void) params;

	std::cout << "Invite command message" << std::endl;
}