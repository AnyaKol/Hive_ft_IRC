#include "Names.hpp"
#include <iostream>

void	Names::execute(const std::set<Channel*>& channels, Client& client,
	const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
	(void) params;

	std::cout << "Names command message" << std::endl;
}