#include "Kick.hpp"
#include <iostream>

void	Kick::execute(std::unordered_set<Channel*>& channels,
	const Client& client, const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
	(void) params;

	std::cout << "Kick command message" << std::endl;
}