#include "Invite.hpp"
#include <iostream>

void	Invite::execute(const std::set<Channel*>& channels, Client& client,
	const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
	(void) params;

	std::cout << "Invite command message" << std::endl;
}