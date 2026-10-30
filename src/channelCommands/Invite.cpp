#include "Invite.hpp"
#include <iostream>

Invite::Invite(Client& client, std::unordered_set<Channel*>& channels)
: AChannelCommand(client, channels) {}

void	Invite::execute(const std::vector<std::string_view>& params) {
	(void) params;

	std::cout << "Invite command message" << std::endl;
}