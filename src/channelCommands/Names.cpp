#include "Names.hpp"
#include <iostream>

void	Names::execute(std::unordered_set<Channel*>& channels,
	const Client& client, const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
	(void) params;

	std::cout << "Names command message" << std::endl;
}