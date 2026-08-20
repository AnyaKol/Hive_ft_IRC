#include "../inc/Commands.hpp"
#include "../inc/Server.hpp"
#include "../inc/Client.hpp"
#include "../inc/Parser.hpp"
#include "../inc/IRC.hpp"
#include <sys/socket.h>
#include <iostream>

// PASS <password>
void Commands::handlePass(Server& serv, Client& client, Parser& cmd) {

    if (cmd.getParams().empty()) {
        std::string reply = IRC::Reply::errNeedMoreParams(client.getNickname(), cmd.getCommand());
        client.appendToWriteBuffer(reply);
        return;
    }

    if (client.isAuthenticated()) {
        std::string reply = IRC::Reply::errAlreadyRegistered(client.getNickname());
        client.appendToWriteBuffer(reply);
        return;
    }

    if (cmd.getParams()[0] == serv.getPassword()) {
        client.setAuthenticated(true);
        std::cout << "Client on socket " << client.getSocket() << " authenticated.\n";
    } else {
        std::string reply = IRC::Reply::errPasswordMismatch(client.getNickname());
        client.appendToWriteBuffer(reply);

        client.setShouldDisconnect(true); // Mark the client for disconnection
        std::cout << "Client on socket " << client.getSocket() << " provided incorrect password. Marked for disconnection.\n";
    }
}

void Commands::handleNick(Server& serv, Client& client, Parser& cmd) {

    if (cmd.getParams().empty()) {
        std::string reply = IRC::Reply::errNeedMoreParams("", cmd.getCommand());
        client.appendToWriteBuffer(reply);
        return;
    }

    const std::string_view nickname = cmd.getParams()[0];

    if (serv.isNicknameTaken(nickname)) {
        std::string reply = IRC::Reply::errNickInUse(client.getNickname(), nickname);
        client.appendToWriteBuffer(reply);
        return;
    }

    if (!client.isAuthenticated()) {
        std::string reply = IRC::Reply::errNotRegistered("");
        client.appendToWriteBuffer(reply);
        return;
    }

    std::string oldNickname = client.getNickname();
    bool wasRegistered = client.isRegistered();

    client.setNickname(nickname);

    std::cout << "Client on socket " << client.getSocket() << " set nickname to: " << client.getNickname() << "\n";

    if (!wasRegistered && client.isRegistered()) { // only print welcome to the user if they were not registred before
        std::string welcomeMsg = IRC::Reply::welcome(client.getNickname(), client.getUsername(), client.getHostname());
        client.appendToWriteBuffer(welcomeMsg);
    }
     else if (wasRegistered) {
        // If they are already registered and changing their name, send the confirmation back to them! so irssi updates the interface
        std::string reply = ":" + oldNickname + "!" + client.getUsername() + "@" + client.getHostname() + " NICK :" + client.getNickname() + "\r\n";
        client.appendToWriteBuffer(reply);

        // (Note: Later when you build Channels, you will also need to broadcast this 'reply' string
        // to every other person in their channels so their screens update too!)
    }

    return;
}

void    Commands::handleUser(Server& serv, Client& client, Parser& cmd) {

    (void)serv;

    if (!client.isAuthenticated()) {
        std::string reply = IRC::Reply::errNotRegistered(client.getNickname());
        client.appendToWriteBuffer(reply);
        return;
    }

    if (!client.getUsername().empty()) {
        std::string reply = IRC::Reply::errAlreadyRegistered(client.getNickname());
        client.appendToWriteBuffer(reply);
        return;
    }

    if (cmd.getParams().size() < 4) { // USER command must have params:  <username> <unused mode> <unused server name> <realname>

        std::string reply = IRC::Reply::errNeedMoreParams(client.getNickname(), cmd.getCommand());
        client.appendToWriteBuffer(reply);
        return;
    }

    client.setUsername(cmd.getParams()[0]);
    // just ignore the unused params.
    client.setRealname(cmd.getParams()[3]);

    if (client.isRegistered()) {
        std::string welcomeMsg = IRC::Reply::welcome(client.getNickname(), client.getUsername(), client.getHostname());
        client.appendToWriteBuffer(welcomeMsg);
    }

    return;
}




// PING <token>
void Commands::handlePing(Server& serv, Client& client, Parser& cmd) {

    (void)serv;
    if (cmd.getParams().empty()) {
        std::string reply = IRC::Reply::errNoOrigin(client.getNickname());
        client.appendToWriteBuffer(reply);
        return;
    }

    std::string reply = "PONG :" + std::string(cmd.getParams()[0]) + "\r\n";
    client.appendToWriteBuffer(reply);
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
    if (!cmd.getParams().empty()) {
        reason = std::string(cmd.getParams()[0]);
    }

    std::string reply = "ERROR :Closing Link: (" + reason + ")\r\n";
    client.appendToWriteBuffer(reply);

    std::cout << "Client on socket " << client.getSocket() << " is quitting: " << reason << "\n";
}