#include "Part.hpp"
#include <iostream>

void	Part::execute(std::unordered_set<Channel*>& channels,
	const Client& client, const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
	(void) params;

	std::cout << "Part command message" << std::endl;
}