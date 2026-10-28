#include "Invite.hpp"
#include <iostream>

void	Invite::execute(std::unordered_set<Channel*>& channels,
	const Client& client, const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
	(void) params;

	std::cout << "Invite command message" << std::endl;
}