#include "Part.hpp"
#include "Channel.hpp"

#include <iostream>

Part::Part(Client& client, std::set<Channel>& channels)
: AChannelCommand(client, channels) {}

void	Part::execute(const std::vector<std::string_view>& params) {
	(void) params;

	std::cout << "Part command message" << std::endl;
}