#include <iostream>
#include <string>
#include <string_view>

#include "Server.hpp"
#include "Channel.hpp"
#include "Client.hpp"

int		main(int ac, char *av[]) {

	if (ac != 3) {
		std::cerr << "Usage: ./ircserv <port> <password>";
		return 	EXIT_FAILURE;
	}

	// parse and validate input
	// init and run server

	return 0;
}