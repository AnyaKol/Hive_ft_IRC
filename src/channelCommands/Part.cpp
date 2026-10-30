#include "Part.hpp"
#include <iostream>

Part::Part(Client& client, std::unordered_set<Channel*>& channels)
: AChannelCommand(client, channels) {}

void	Part::execute(const std::vector<std::string_view>& params) {
	(void) params;

	std::cout << "Part command message" << std::endl;
}