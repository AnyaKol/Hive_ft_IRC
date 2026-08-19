#include "../inc/Commands.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Parser.hpp"
#include "../inc/IRC.hpp"
#include <sys/socket.h>
#include <iostream>

// PASS <password>
void Commands::handlePass(Server& serv, Client& client, Parser& cmd) {

    if (cmd._params.empty()) {
        std::string reply = IRC::Reply::errNeedMoreParams(client.getNickname(), cmd._command);
        send(client.getSocket(), reply.c_str(), reply.length(), 0);
        return;
    }

    if (client.isAuthenticated()) {
        std::string reply = IRC::Reply::errAlreadyRegistered(client.getNickname());
        send(client.getSocket(), reply.c_str(), reply.length(), 0);
        return;
    }

    if (cmd._params[0] == serv.getPassword()) {
        client.setAuthenticated(true);
        std::cout << "Client on socket " << client.getSocket() << " authenticated.\n";
    } else {
        std::string reply = IRC::Reply::errPasswordMismatch(client.getNickname());
        send(client.getSocket(), reply.c_str(), reply.length(), 0);
    }
}

// PING <token>
void Commands::handlePing(Server& serv, Client& client, Parser& cmd) {

    (void)serv; 
    if (cmd._params.empty()) {
        std::string reply = IRC::Reply::errNoOrigin(client.getNickname());
        send(client.getSocket(), reply.c_str(), reply.length(), 0);
        return;
    }

    std::string reply = "PONG :" + std::string(cmd._params[0]) + "\r\n";
    send(client.getSocket(), reply.c_str(), reply.length(), 0);
}

// PONG <token>
void Commands::handlePong(Server& serv, Client& client, Parser& cmd) {

    (void)serv;
    (void)cmd;
    std::cout << "Received PONG from client on socket " << client.getSocket() << "\n";
}

// QUIT [Quit Message]
void Commands::handleQuit(Server& serv, Client& client, Parser& cmd) {

    (void)serv;
    std::string reason = "Client quit";
    if (!cmd._params.empty()) {
        reason = std::string(cmd._params[0]);
    }

    std::string reply = "ERROR :Closing Link: (" + reason + ")\r\n";
    send(client.getSocket(), reply.c_str(), reply.length(), 0);

    std::cout << "Client on socket " << client.getSocket() << " is quitting: " << reason << "\n";
}