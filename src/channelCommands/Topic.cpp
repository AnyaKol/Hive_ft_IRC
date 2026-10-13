#include "Topic.hpp"
#include <iostream>

void	Topic::execute(Client& client) {
	(void) client;
	std::cout << "Topic command message" << std::endl;
}