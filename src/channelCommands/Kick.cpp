#include "Kick.hpp"
#include <iostream>

void	Kick::execute(Client& client) {
	(void) client;
	std::cout << "Kick command message" << std::endl;
}