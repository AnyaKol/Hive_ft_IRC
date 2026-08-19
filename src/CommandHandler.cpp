#include "../inc/Commands.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Parser.hpp"
#include <sys/socket.h>
#include <iostream>

void Commands::processCommand(Server& server, Client& client, Parser& cmd) {
    
    // Authentication Commands
    if (cmd.command == "PASS") {
        handlePass(server, client, cmd);
    }
    else if (cmd.command == "PING") {
        handlePing(server, client, cmd);
    }
    else if (cmd.command == "PONG") {
        handlePong(server, client, cmd);
    }
    else if (cmd.command == "QUIT") {
        handleQuit(server, client, cmd);
    }
    
    // Channel Commands (Add later)
    // else if (cmd.command == "JOIN")...

    // THE CATCH-ALL: Command not found
    else {
        std::string reply = "421 " + std::string(cmd.command) + " :Unknown command\r\n";
        send(client.getSocket(), reply.c_str(), reply.length(), 0);
        std::cout << "Client on socket " << client.getSocket() << " sent unknown command: " << cmd.command << "\n";
    }
}