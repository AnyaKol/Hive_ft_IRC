#include "Client.hpp"

Client::Client(Server& server, int socket) : _server{server}, _socketFd{socket} {}

void	Client::setFd(int fd) { this->_socketFd = fd; }

int	Client::getFd() const { return this->_socketFd; }

void	Client::setIP(const std::string& ip) { this->_ipAddress = ip; }

const std::string&	Client::getIP() const { return this->_ipAddress; }
