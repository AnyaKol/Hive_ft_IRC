#pragma once

class Server;
class Client;
class Parser;

namespace Commands {
    
    void processCommand(Server& server, Client& client, Parser& cmd);

    void handlePass(Server& server, Client& client, Parser& cmd);
    void handlePing(Server& server, Client& client, Parser& cmd);
    void handlePong(Server& server, Client& client, Parser& cmd);
    void handleQuit(Server& server, Client& client, Parser& cmd);

}