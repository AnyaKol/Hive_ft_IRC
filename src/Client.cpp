#include "../inc/Client.hpp"

Client::Client(int socket) : _socketFd{socket} {}


bool	Client::hasCompleteCommand() const {
	// Check if the buffer contains a complete command (looks for \n)
	return _readBuffer.find('\n') != std::string::npos;
}

std::string Client::extractCommandFromBuffer() {

	// Find the position of the first complete command (ending with \n)
	size_t pos = _readBuffer.find('\n');
	if (pos != std::string::npos) {
		size_t end_pos = pos;
		size_t erase_len = pos + 1;

		// Check if it's \r\n or just \n
		if (pos > 0 && _readBuffer[pos - 1] == '\r') {
			end_pos = pos - 1;
		}

		std::string command = _readBuffer.substr(0, end_pos);
		_readBuffer.erase(0, erase_len);
		return command; // return it stripped out the \r\n from TCP protocol
	}
	return "";
}

bool	Client::isRegistered() const {

	return _isAuthenticated && !_nickname.empty() && !_username.empty();
}

void Client::joinChannel() {}
void Client::leaveChannel() {}