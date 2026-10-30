#include "Mode.hpp"
#include <iostream>

Mode::Mode(Client& client, std::unordered_set<Channel*>& channels)
: AChannelCommand(client, channels) {}

void	Mode::execute(const std::vector<std::string_view>& params) {
	(void) params;

	std::cout << "Mode command message" << std::endl;
}