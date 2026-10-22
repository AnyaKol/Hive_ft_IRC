#include "Part.hpp"
#include <iostream>

void	Part::execute(const std::set<Channel*>& channels, Client& client,
	const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
	(void) params;

	std::cout << "Part command message" << std::endl;
}