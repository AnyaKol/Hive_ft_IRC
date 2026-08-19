#include "../inc/Commands.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Parser.hpp"
#include <sys/socket.h>
#include <iostream>

// PASS <password>
void Commands::handlePass(Server& server, Client& client, Parser& cmd) {
    if (cmd.params.empty()) {
        std::string reply = "461 :Not enough parameters\r\n";
        send(client.getSocket(), reply.c_str(), reply.length(), 0);
        return;
    }

    if (client.isAuthenticated()) {
        std::string reply = "462 :Unauthorized command (already registered)\r\n";
        send(client.getSocket(), reply.c_str(), reply.length(), 0);
        return;
    }

    if (cmd.params[0] == server.getPassword()) {
        client.setAuthenticated(true);
        std::cout << "Client on socket " << client.getSocket() << " authenticated.\n";
    } else {
        std::string reply = "464 :Password incorrect\r\n";
        send(client.getSocket(), reply.c_str(), reply.length(), 0);
    }
}

// PING <token>
void Commands::handlePing(Server& server, Client& client, Parser& cmd) {
    (void)server;
    if (cmd.params.empty()) {
        std::string reply = "409 :No origin specified\r\n";
        send(client.getSocket(), reply.c_str(), reply.length(), 0);
        return;
    }

    std::string reply = "PONG :" + std::string(cmd.params[0]) + "\r\n";
    send(client.getSocket(), reply.c_str(), reply.length(), 0);
}

// PONG <token>
void Commands::handlePong(Server& server, Client& client, Parser& cmd) {
    (void)server;
    (void)cmd;
    std::cout << "Received PONG from client on socket " << client.getSocket() << "\n";
}

// QUIT [Quit Message]
void Commands::handleQuit(Server& server, Client& client, Parser& cmd) {
    (void)server;
    std::string reason = "Client quit";
    if (!cmd.params.empty()) {
        reason = std::string(cmd.params[0]);
    }

    std::string reply = "ERROR :Closing Link: (" + reason + ")\r\n";
    send(client.getSocket(), reply.c_str(), reply.length(), 0);

    std::cout << "Client on socket " << client.getSocket() << " is quitting: " << reason << "\n";
}