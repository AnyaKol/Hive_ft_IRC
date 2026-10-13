#include "Invite.hpp"
#include <iostream>

void	Invite::execute(Client& client) {
	(void) client;
	std::cout << "Invite command message" << std::endl;
}