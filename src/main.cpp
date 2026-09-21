#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <charconv>
#include <system_error>
#include <cstdlib>

#include "Server.hpp"

int main(int ac, char *av[]) {

    if (ac != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>\n";
        return EXIT_FAILURE;
    }

    // maybe we can do the input validation in one helper function to make main cleaner.
    std::string password{av[2]};
    if (password.empty()) {
        std::cerr << "Error: Password cannot be empty!\n";
        return EXIT_FAILURE;
    }

    // TODO: Add strict validation here later (check for letters, symbols, or invalid input).
    // For now, we are just doing a basic conversion to keep it simple.
    int port = std::atoi(av[1]);

    if (port < 1024 || port > 65535) {
        std::cerr << "Error: Port number out of accepted range. Must be 1024 - 65535.\n";
        return EXIT_FAILURE;
    }

    Server  server(port, password);
    try {
        signal(SIGINT, Server::signalHandler);
        signal(SIGQUIT, Server::signalHandler);
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