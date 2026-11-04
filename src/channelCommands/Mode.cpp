#include "Mode.hpp"
#include <iostream>

<<<<<<< HEAD
Mode::Mode(Client& client, std::unordered_set<Channel*>& channels)
: AChannelCommand(client, channels) {}

void	Mode::execute(const std::vector<std::string_view>& params) {
=======
void	Mode::execute(std::unordered_set<Channel*>& channels,
	const Client& client, const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
>>>>>>> antor
	(void) params;

	std::cout << "Mode command message" << std::endl;
}