#include "Invite.hpp"
#include <iostream>

<<<<<<< HEAD
Invite::Invite(Client& client, std::unordered_set<Channel*>& channels)
: AChannelCommand(client, channels) {}

void	Invite::execute(const std::vector<std::string_view>& params) {
=======
void	Invite::execute(std::unordered_set<Channel*>& channels,
	const Client& client, const std::vector<std::string_view>& params) {
	(void) channels;
	(void) client;
>>>>>>> antor
	(void) params;

	std::cout << "Invite command message" << std::endl;
}