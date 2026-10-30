#include "Names.hpp"
#include <iostream>

Names::Names(Client& client, std::unordered_set<Channel*>& channels)
: AChannelCommand(client, channels) {}

void	Names::execute(const std::vector<std::string_view>& params) {
	(void) params;

	std::cout << "Names command message" << std::endl;
}