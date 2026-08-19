#include "Server.hpp"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

bool	Server::_signal = false;

void	Server::signalHandler(int sig) {

	(void)sig;
	std::cout << "\nSignal Arrived!\n";
	Server::_signal = true;
}

Server::Server(std::uint16_t port, const std::string& password) : _port{port}, _password{password}, _serverSocket{-1} {

}


void	Server::initServer() {

	int	ret{};
	struct pollfd server_pollfd;

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

	server_pollfd.fd = this->_serverSocket;
	server_pollfd.events = POLLIN; // set the event to listen for incoming data (POLLIN)
	this->_fds.push_back(server_pollfd);
}

void Server::runServer() {

    while (!_signal) { // while the server is running and no signal has been received

       	if (poll(&_fds[0], _fds.size(), -1) == -1) // -1 = block until an event occur
			throw std::runtime_error("Poll failed");

        for (size_t i = 0; i < _fds.size(); i++) { // check all our sockets to see who has the event

            if (_fds[i].revents & POLLIN) { // checks if the POLLIN event is returned

                if (_fds[i].fd == _serverSocket) { // the event is from the server socket, meaning new client is connecting
					acceptClient(); // -> Call accept(), make it non-blocking, and push to 'fds' vector
                } else {
                    // else then the event is from the client socket (They sent us an IRC command!)
					receiveData(_fds[i].fd); // -> Call recv() to read the message and parse it
                }
            }
        }
    }
	closeAll();
}

void	Server::closeAll() {

	for (Client &client : _clients) { // need to also clear the client contianer later..
		close(client.getFd());
	}
	if (-1 != _serverSocket) {
		close (_serverSocket);
	}
}


