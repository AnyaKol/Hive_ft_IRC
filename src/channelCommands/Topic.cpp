#include "Topic.hpp"
#include <iostream>

void	Topic::execute(std::unordered_set<Channel*>& channels,
	const Client& client, const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
	(void) params;

	std::cout << "Topic command message" << std::endl;
}