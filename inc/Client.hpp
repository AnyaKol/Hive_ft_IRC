#pragma once

#include <string>
#include <string_view>
#include "Channel.hpp"

class Server;

class Client {

    private:

        std::string _username {};
        std::string _nickname {};
        std::string _hostname {};
        std::string _password {};
        std::string _buffer {}; // to store client input data and append to it / partial data
        std::string _ipAddress {};
        int         _socketFd {-1};
        bool        _isAuthenticated = false;

    public:
        Client() = default;
        Client(const Client&) = default;
        Client(int socket);
        ~Client() = default;

        Client&     operator=(const Client&) = default;


        const   std::string&    getUsername() const;
        const   std::string&    getNickname() const;
        const   std::string&    getHostname() const;
        const   std::string&    getIP() const;
        int     getFd() const;

        // Alias so your AuthCommands.cpp still works perfectly
        int     getSocket() const { return _socketFd; }

        void    setFd(int fd);
        void    setIP(const std::string& ip);
        void    setUsername(const std::string_view username);
        void    setNickname(const std::string_view nickname);
        void    setHostname(const std::string_view hostname);

        void    joinChannel();
        void    leaveChannel();

        // Your authentication getters and setters
        bool    isAuthenticated() const { return _isAuthenticated; }
        void    setAuthenticated(bool status) { _isAuthenticated = status; }

        void    appendToBuffer(const std::string& data);
        const   std::string&    getBuffer() const;

        bool    hasCompleteCommand() const;
        std::string extractCommandFromBuffer();



};