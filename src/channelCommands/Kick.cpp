#include "Kick.hpp"
#include <iostream>

void	Kick::execute(const std::set<Channel*>& channels, Client& client,
	const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
	(void) params;

	std::cout << "Kick command message" << std::endl;
}