#include "../inc/Client.hpp"

Client::Client(int socket) : _socketFd{socket} {}

void	Client::setFd(int fd) { this->_socketFd = fd; }

int	Client::getFd() const { return this->_socketFd; }

void	Client::setIP(const std::string& ip) { this->_ipAddress = ip; }

const std::string&	Client::getIP() const { return this->_ipAddress; }


void    Client::appendToBuffer(const std::string& data) { _buffer += data; }

const   std::string&    Client::getBuffer() const { return _buffer; }

bool	Client::hasCompleteCommand() const {
	// Check if the buffer contains a complete command (looks for \n)
	return _buffer.find('\n') != std::string::npos;
}

std::string Client::extractCommandFromBuffer() {

	// Find the position of the first complete command (ending with \n)
	size_t pos = _buffer.find('\n');
	if (pos != std::string::npos) {
		size_t end_pos = pos;
		size_t erase_len = pos + 1;

		// Check if it's \r\n or just \n
		if (pos > 0 && _buffer[pos - 1] == '\r') {
			end_pos = pos - 1;
		}

		std::string command = _buffer.substr(0, end_pos);
		_buffer.erase(0, erase_len);
		return command; // stripped out the \r\n from TCP protocol
	}
	return "";
}

const std::string& Client::getUsername() const { return _username; }
const std::string& Client::getNickname() const { return _nickname; }
const std::string& Client::getHostname() const { return _hostname; }

void Client::setUsername(const std::string_view username) { _username = username; }
void Client::setNickname(const std::string_view nickname) { _nickname = nickname; }
void Client::setHostname(const std::string_view hostname) { _hostname = hostname; }

void Client::joinChannel() {}
void Client::leaveChannel() {}