#include "../inc/Server.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

bool	Server::_signal = false;

void	Server::signalHandler(int sig) {

	(void)sig;
	std::cout << "\nSignal Arrived!\n";
	Server::_signal = true;
}

Server::Server(std::uint16_t port, std::string& password) : _port{port}, _password{password}, _serverSocket{-1} {

}


bool	Server::initServer() {

	int	ret{};
	struct pollfd server_pollfd;

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

	server_pollfd.fd = this->_serverSocket;
	server_pollfd.events = POLLIN; // set the event to listen for incoming data (POLLIN)
	server_pollfd.revents = 0;
	this->_pollfds.push_back(server_pollfd);

	return true;

}

void Server::runServer() {

    while (!_signal) { // while the server is running and no signal has been received

       	if (poll(&_pollfds[0], _pollfds.size(), -1) == -1) { // -1 = block until an event occur
			if (!_signal) {
				std::cerr << "Error: Poll failed!" << std::endl;
			}
			break;
		}

        for (size_t i = 0; i < _pollfds.size(); i++) { // check all our sockets to see who has the event

            if (_pollfds[i].revents & POLLIN) { // checks if the POLLIN read event is ready

                if (_pollfds[i].fd == _serverSocket) { // the event is from the server socket, meaning new client is connecting
					acceptClient(); // -> Call accept(), make it non-blocking, and push to 'fds' vector
                } else {
                    // else then the event is from the client socket (They sent us an IRC command!)
					size_t current_size = _pollfds.size();
					receiveData(_pollfds[i].fd); // -> Call recv() to read the message and parse it
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

	if ((clientFd = accept(this->_serverSocket, (sockaddr*)&clientAddress, &len)) == -1) {
		std::cerr << "Error: Can't accept the new client!" << std::endl;
		return;
	}

	Client	newClient(*this, clientFd);

	if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "Error: Failed to set socket to non-blocking mode!" << std::endl;
		close(clientFd);
		return;
	}

	client_pollfd.fd = clientFd;
	client_pollfd.events = POLLIN;
	client_pollfd.revents = 0;

	newClient.setIP(inet_ntop(AF_INET, &clientAddress.sin_addr, clientIP, INET_ADDRSTRLEN)); // Convert the client's IP address to a string and store it in the Client object
	this->_clients.insert(std::make_pair(clientFd, newClient)); // Add the new client to the hash map using their fd as the key
	this->_pollfds.push_back(client_pollfd);

	// log that the client is connected..

}

void	Server::receiveData(int fd) {

	char	buffer[MAXLINE] {};

	ssize_t received_data = recv(fd, buffer, MAXLINE -1, 0);
	if (received_data <= 0) {
		clearClient(fd);
		return;
	}

	// parse the data and process it.
}

void Server::clearClient(int fd) {

	for (std::vector<struct pollfd>::iterator it = _pollfds.begin(); it != _pollfds.end(); ++it) {
		if (it->fd == fd) {
			_pollfds.erase(it); // careful with erase it shift to the left.. need tto keep track of correct num of clients
			break;
		}
	}

	_clients.erase(fd); // O(1) instant removal from the hash map!

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

