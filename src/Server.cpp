#include "../inc/Server.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <Parser.hpp>
#include <Commands.hpp>

bool	Server::_signal = false;

void	Server::signalHandler(int sig) {

	(void)sig;
	std::cout << "\nSignal Arrived!\n";
	Server::_signal = true;
}

Server::Server(std::uint16_t port, const std::string& password) : _port{port}, _password{password}, _serverSocket{-1} { }

bool	Server::initServer() {

	int	ret{};
	struct pollfd server_pollfd;

	_serverLog.open("BackLog.txt", std::ofstream::trunc);
	if (!_serverLog.is_open()) {
		std::cerr << "Failed to open server log file." << std::endl;
		return false;
	}

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
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
	setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &socketoption, sizeof(socketoption));


	sockaddr_in serverAddress{};
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(_port);

	ret = bind(_serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
	if (-1 == ret) {
		std::cerr << "Failed to bind socket." << std::endl;
		return false;
	}

	ret = listen(_serverSocket, SOMAXCONN);
	if (-1 == ret) {
		std::cerr << "Failed to listen on socket." << std::endl;
		return false;
	}

	server_pollfd.fd = this->_serverSocket;
	server_pollfd.events = POLLIN;
	server_pollfd.revents = 0;
	this->_pollfds.push_back(server_pollfd);

	return true;

}

void Server::runServer() {

    while (!_signal) {

       	if (poll(&_pollfds[0], _pollfds.size(), -1) == -1) { // block until an event occur
			if (!_signal) {
				std::cerr << "Error: Poll failed!" << std::endl;
			}
			break;
		}

        for (size_t i = 0; i < _pollfds.size(); i++) {

            if (_pollfds[i].revents & POLLIN) { // checks if the POLLIN read event is ready

                if (_pollfds[i].fd == _serverSocket) { // the event is from the server socket, meaning new client is connecting
					acceptClient();
                } else {
                    // else then the event is from the client socket (They sent us an IRC command!)
					size_t current_size = _pollfds.size();
					receiveData(_pollfds[i].fd);
					if (_pollfds.size() < current_size) {
						i--; // Decrement i so we don't skip the element that just shifted left from clearClient.
					}
                }
            }
        }
    }
	closeAll();
}

void	Server::acceptClient() {

	struct	sockaddr_in	clientAddress{};
	struct 	pollfd		client_pollfd{};
	int		clientFd{};
	char	clientIP[INET_ADDRSTRLEN + 1];
	socklen_t	len = sizeof(clientAddress);

	if ((clientFd = accept(this->_serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &len)) == -1) {
		std::cerr << "Error: Can't accept the new client!" << std::endl;
		return;
	}

	Client	newClient(clientFd);

	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "Error: Failed to set socket to non-blocking mode!" << std::endl;
		close(clientFd);
		return;
	}

	client_pollfd.fd = clientFd;
	client_pollfd.events = POLLIN;
	client_pollfd.revents = 0;

	if (inet_ntop(AF_INET, &clientAddress.sin_addr, clientIP, INET_ADDRSTRLEN))
		newClient.setIP(clientIP);
	else {
		std::cerr << "Error: Failed to convert client IP address!" << std::endl;
		close(clientFd);
		return;
	}

	this->_clients.insert(std::make_pair(clientFd, newClient)); // Add the new client to the hash map using their fd as the key
	this->_pollfds.push_back(client_pollfd);

	std::cout << "Client (FD " << clientFd << ") connected from IP: " << newClient.getIP() << std::endl;

}

void	Server::receiveData(int fd) {

	char	buffer[MAXLINE] {};

	ssize_t received_data = recv(fd, buffer, MAXLINE -1, 0);
	if (received_data <= 0) {
		clearClient(fd);
		return;
	}

	// parse the data and process it.
	_clients[fd].appendToBuffer(buffer);

	while (_clients[fd].hasCompleteCommand()) { // we can have multiple commands in the buffer, so we need to process them all

		std::string command = _clients[fd].extractCommandFromBuffer();
		Parser cmd(command);
		Commands::processCommand(*this, _clients[fd], cmd);
	}
}

void Server::clearClient(int fd) {

	for (std::vector<struct pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); ++it) {
		if (it->fd == fd) {
			_pollfds.erase(it); // careful with erase it shift to the left.. need to keep track of correct num of clients
			break;
		}
	}

	_clients.erase(fd);

	close(fd);
	std::cout << "Client (FD " << fd << ") disconnected." << std::endl;
}

void	Server::closeAll() {

	for (std::unordered_map<int, Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
		close(it->second.getFd()); // first is the fd key, second is the client obj
	}
	if (-1 != _serverSocket) {
		close (_serverSocket);
	}
}

const std::string& Server::getPassword() {

	return _password;
}
