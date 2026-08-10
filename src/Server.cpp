#include "../inc/Server.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

Server::Server(std::uint16_t port, const std::string& password) : _port{port}, _password{password}, _pollFd{-1}, _serverSocket{-1} {

}


bool	Server::initServer() {

	int	ret{};

	_serverLog.open("BackLog.txt", std::ofstream::trunc); // Truncate an existing stream when opening.
	if (!_serverLog.is_open()) {
		std::cerr << "Failed to open server log file." << std::endl;
		return false;
	}

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0); // Create a TCP ipv4 socket
	if (-1 == _serverSocket) {
		std::cerr << "Failed to create socket." << std::endl;
		return false;
	}

	ret = fcntl(_serverSocket, F_SETFL, O_NONBLOCK); // Set the socket to non-blocking mode
	if (-1 == ret) {
		std::cerr << "Failed to set socket to non-blocking mode." << std::endl;
		return false;
	}

	int socketoption = true;
	setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &socketoption, sizeof(socketoption)); // Allow reuse of local addresses, act on the socket level, and set the option to true


	sockaddr_in serverAddress{};
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address
	serverAddress.sin_family = AF_INET; // IPv4
	serverAddress.sin_port = htons(_port); // Convert port number to network short byte order 16 bits

	ret = bind(_serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)); // Bind the socket to the specified address and port
	if (-1 == ret) {
		std::cerr << "Failed to bind socket." << std::endl;
		return false;
	}

	ret = listen(_serverSocket, SOMAXCONN); // Start listening for incoming connections
	if (-1 == ret) {
		std::cerr << "Failed to listen on socket." << std::endl;
		return false;
	}

	// Create a poll file descriptor for monitoring events on the server socket
	// using poll or epoll or select system calls
	return true;

}

