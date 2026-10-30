#include "Topic.hpp"
#include <iostream>

Topic::Topic(Client& client, std::unordered_set<Channel*>& channels)
: AChannelCommand(client, channels) {}

void	Topic::execute(const std::vector<std::string_view>& params) {
	(void) params;

	std::cout << "Topic command message" << std::endl;
}