#pragma once

class Server;
class Client;
class Parser;

namespace Commands {

    void processCommand(Server& serv, Client& client, Parser& cmd);

    void handlePass(Server& serv, Client& client, Parser& cmd);
    void handlePing(Server& serv, Client& client, Parser& cmd);
    void handlePong(Server& serv, Client& client, Parser& cmd);
    void handleQuit(Server& serv, Client& client, Parser& cmd);

}