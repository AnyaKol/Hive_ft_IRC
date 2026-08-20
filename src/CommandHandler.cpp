#include "../inc/Commands.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Parser.hpp"
#include <sys/socket.h>
#include <iostream>
#include "../inc/IRC.hpp"

void Commands::processCommand(Server& serv, Client& client, Parser& cmd) {

    std::string command = cmd.getCommand();

    // Allow registeration commands to go through unconditionally
    if (command == "PASS") { handlePass(serv, client, cmd); return; }
    if (command == "NICK") { handleNick(serv, client, cmd); return; }
    if (command == "USER") { handleUser(serv, client, cmd); return; }
    if (command == "CAP") { return; }  /* whitelisting CAP so the client can be happy! */


    // If they are not registered, block all other commands
    if (!client.isRegistered()) {
        std::string reply = IRC::Reply::errNotRegistered(client.getNickname());
        client.appendToWriteBuffer(reply);
        return;
    }

    if (command == "PING") { handlePing(serv, client, cmd); return; }
    if (command == "PONG") { handlePong(serv, client, cmd); return; }
    if (command == "QUIT") { handleQuit(serv, client, cmd); return; }


    // Channel Commands (Add later)

    // THE CATCH-ALL: Command not found
    if (command == "UNKNOWN") {

        std::string reply = IRC::Reply::errUnknownCommand(client.getNickname(), cmd.getCommand());
        client.appendToWriteBuffer(reply);
    }
}