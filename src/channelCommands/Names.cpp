#include "Names.hpp"
#include <iostream>

void	Names::execute(Client& client) {
	(void) client;
	std::cout << "Names command message" << std::endl;
}