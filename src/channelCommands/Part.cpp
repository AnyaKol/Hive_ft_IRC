#include "Part.hpp"
#include <iostream>

<<<<<<< HEAD
Part::Part(Client& client, std::unordered_set<Channel*>& channels)
: AChannelCommand(client, channels) {}

void	Part::execute(const std::vector<std::string_view>& params) {
=======
void	Part::execute(std::unordered_set<Channel*>& channels,
	const Client& client, const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
>>>>>>> antor
	(void) params;

	std::cout << "Part command message" << std::endl;
}