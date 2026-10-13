#include "Part.hpp"
#include <iostream>

void	Part::execute(Client& client) {
	(void) client;
	std::cout << "Part command message" << std::endl;
}