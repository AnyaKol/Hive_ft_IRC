#include "Server.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

Server::Server(std::uint16_t port, const std::string& password) : _port{port}, _password{password}, _pollFd{-1}, _serverSocket{-1} {

}


void	Server::initServer() {

	int	ret{};

	this->_serverLog.open("BackLog.txt", std::ofstream::trunc); // Truncate an existing stream when opening.
	if (!this->_serverLog.is_open())
		throw ( std::runtime_error("Failed to open server log file.") );

	this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP ipv4 socket
	if (-1 == this->_serverSocket)
		throw ( std::runtime_error("Failed to create socket.") );

	ret = fcntl(this->_serverSocket, F_SETFL, O_NONBLOCK); // Set the socket to non-blocking mode
	if (-1 == ret)
		throw ( std::runtime_error("Failed to set socket to non-blocking mode.") );

	int socketoption = 1;
	ret = setsockopt(this->_serverSocket, SOL_SOCKET, SO_REUSEADDR, &socketoption, sizeof(socketoption)); // Allow reuse of local addresses, act on the socket level, and set the option to true
	if (-1 == ret)
		throw ( std::runtime_error("Failed to set option to reuse local addresses.") );


	sockaddr_in serverAddress{};
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address
	serverAddress.sin_family = AF_INET; // IPv4
	serverAddress.sin_port = htons(this->_port); // Convert port number to network short byte order 16 bits

	ret = bind(this->_serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)); // Bind the socket to the specified address and port
	if (-1 == ret)
		throw ( std::runtime_error("Failed to bind socket.") );

	ret = listen(this->_serverSocket, SOMAXCONN); // Start listening for incoming connections and make socket a passive socket.
	if (-1 == ret)
		throw ( std::runtime_error("Failed to listen on socket.") );

	// Create a poll file descriptor for monitoring events on the server socket

	// using poll or epoll or select system calls


}

