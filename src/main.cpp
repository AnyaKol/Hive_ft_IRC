#include "Server.hpp"

#include <exception>
#include <iostream>
#include <csignal>
#include <cstdlib>


int main(int ac, char *av[]) {

	if (ac != 3) {
		std::cerr << "Usage: ./ircserv <port> <password>\n";
		return EXIT_FAILURE;
	}

	
	std::string password{av[2]};
	if (password.empty()) {
		std::cerr << "Error: Password cannot be empty!\n";
		return EXIT_FAILURE;
	}

	std::string	portNUm(av[1]);
	if (portNUm.size() > 9 || portNUm.find_first_not_of("0123456789") != std::string_view::npos) {
		std::cerr << "Error: Invalid port number.\n";
		return EXIT_FAILURE;
	}
	int port = std::atoi(av[1]);

	if (port < 1024 || port > 65535) {
		std::cerr << "Error: Port number out of accepted range. Must be 1024 - 65535.\n";
		return EXIT_FAILURE;
	}

	Server  server(port, password);
	try {
		signal(SIGINT, Server::signalHandler); // Ctrl + 'C'
		signal(SIGQUIT, Server::signalHandler); // Ctrl + '\'
		signal(SIGTERM, Server::signalHandler); // Signal sent by kill <pid>
		signal(SIGPIPE, SIG_IGN); // Ignore SIGPIPE to prevent server crash when writing to a closed socket
		if (server.initServer()) {
		std::cout << "Server initialized successfully, waiting for connections..\n";
		server.runServer();
		} else {
			std::cerr << "Failed to initialize the server!\n";
		}
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return  EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}