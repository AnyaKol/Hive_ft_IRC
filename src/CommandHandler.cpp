#include "../inc/Commands.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Parser.hpp"
#include <sys/socket.h>
#include <iostream>

void Commands::processCommand(Server& serv, Client& client, Parser& cmd) {

    // Authentication Commands
    if (cmd._command == "PASS") {
        handlePass(serv, client, cmd);
    }
    else if (cmd._command == "PING") {
        handlePing(serv, client, cmd);
    }
    else if (cmd._command == "PONG") {
        handlePong(serv, client, cmd);
    }
    else if (cmd._command == "QUIT") {
        handleQuit(serv, client, cmd);
    }

    // Channel Commands (Add later)
    // else if (cmd._command == "JOIN")...

    // THE CATCH-ALL: Command not found
    else {
        std::string reply = "421 " + std::string(cmd._command) + " :Unknown command\r\n";
        send(client.getSocket(), reply.c_str(), reply.length(), 0);
        std::cout << "Client on socket " << client.getSocket() << " sent unknown command: " << cmd._command << "\n";
    }
}