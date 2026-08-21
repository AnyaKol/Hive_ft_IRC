#pragma once

#include <string>
#include <string_view>
#include "Channel.hpp"

class Server;

class Client {

    private:

        std::string _username {};
        std::string _realname {};
        std::string _nickname {};
        std::string _hostname {};
        std::string _readBuffer {}; // to store client input data and append to it / partial data
        std::string _writeBuffer {}; // check with POLLOUT in runServer to make sure that the socket is ready to write
        std::string _password {};
        std::string _ipAddress {};
        int         _socketFd {-1};
        bool        _isAuthenticated = false;
        bool        _shouldDisconnect = false;

    public:
        Client() = default;
        Client(const Client&) = default;
        Client(int socket);
        ~Client() = default;

        Client&     operator=(const Client&) = default;


        const   std::string&    getRealname() const { return _realname; };
        const   std::string&    getUsername() const { return _username; };
        const   std::string&    getNickname() const { return _nickname; };
        const   std::string&    getHostname() const { return _hostname; };
        const   std::string&    getIP() const { return _ipAddress; };
        const   std::string&    getReadBuffer() const {return _readBuffer;}
        const   std::string&    getWriteBuffer() const {return _writeBuffer;}
        int     getSocket() const { return _socketFd; }
        bool    shouldDisconnect() const { return _shouldDisconnect; }

        void    setSocket(int fd) { this->_socketFd = fd; }
        void    setIP(const std::string& ip) { _ipAddress = ip; }
        void    setRealname(const std::string_view realname) { _realname = realname; }
        void    setUsername(const std::string_view username) { _username = username; }
        void    setNickname(const std::string_view nickname) { _nickname = nickname; }
        void    setHostname(const std::string_view hostname) { _hostname = hostname; }
        void    setShouldDisconnect(bool status) { _shouldDisconnect = status; }

        void    joinChannel();
        void    leaveChannel();

        // Your authentication getters and setters
        bool    isAuthenticated() const { return _isAuthenticated; }
        void    setAuthenticated(bool status) { _isAuthenticated = status; }


        bool    hasCompleteCommand() const;
        std::string extractCommandFromBuffer();

        void    appendToReadBuffer(const std::string& data) { _readBuffer += data; }
        // use it to avoid using send directly from the command handlers, append to the buffer and let the server handle the sending so no partial sends happens..
        void    appendToWriteBuffer(const std::string& message) {_writeBuffer += message; }
        void    eraseFromWriteBuffer(size_t len) { _writeBuffer.erase(0, len); }


        bool isRegistered() const;


};