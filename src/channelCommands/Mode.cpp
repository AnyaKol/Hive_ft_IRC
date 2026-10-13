#include "Mode.hpp"
#include <iostream>

void	Mode::execute(Client& client) {
	(void) client;
	std::cout << "Mode command message" << std::endl;
}