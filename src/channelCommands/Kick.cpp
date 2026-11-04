#include "Kick.hpp"
#include <iostream>

<<<<<<< HEAD
Kick::Kick(Client& client, std::unordered_set<Channel*>& channels)
: AChannelCommand(client, channels) {}

void	Kick::execute(const std::vector<std::string_view>& params) {
=======
void	Kick::execute(std::unordered_set<Channel*>& channels,
	const Client& client, const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
>>>>>>> antor
	(void) params;

	std::cout << "Kick command message" << std::endl;
}