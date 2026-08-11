#include <iostream>
#include <string>
#include <string_view>
#include <charconv>
#include <system_error>
#include <cstdlib>

#include "Server.hpp"

int main(int ac, char *av[]) {
    
    // 1. The Argument Check
    if (ac != 3) {
        std::cerr << "Usage: ./ircserv <port> <password>\n";
        return EXIT_FAILURE;
    }

    // 2. The Password Check
    std::string_view password{av[2]};
    if (password.empty()) {
        std::cerr << "Error: Password cannot be empty!\n";
        return EXIT_FAILURE;
    }

// 3. The Port Conversion
    // TODO: Add strict validation here later (check for letters, symbols, or invalid input).
    // For now, we are just doing a basic conversion to keep it simple.
    int port = std::atoi(av[1]);

    // 4. The Port Range Check
    if (port < 1024 || port > 65535) {
        std::cerr << "Error: Port number out of accepted range. Must be 1024 - 65535.\n";
        return EXIT_FAILURE;
    }

    // 5. Server Initialization
    Server irc_server(port, std::string(password));
    
    try {
        irc_server.initServer();
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Error: Failed to setup server." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Server started successfully! Listening on port " << port << "..." << std::endl;

    // 6. The Main Loop
    /* if (!irc_server.runServer()) {
        std::cerr << "Error: Server loop terminated unexpectedly.\n";
        return EXIT_FAILURE;
    }
	*/

    return EXIT_SUCCESS;
}