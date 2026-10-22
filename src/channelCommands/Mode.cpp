#include "Mode.hpp"
#include <iostream>

void	Mode::execute(const std::set<Channel*>& channels, Client& client,
	const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
	(void) params;

	std::cout << "Mode command message" << std::endl;
}