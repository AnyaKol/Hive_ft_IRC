#include "Topic.hpp"
#include <iostream>

void	Topic::execute(const std::set<Channel*>& channels, Client& client,
	const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
	(void) params;

	std::cout << "Topic command message" << std::endl;
}