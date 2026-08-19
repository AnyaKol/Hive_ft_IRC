#pragma	once

#include <string>
#include <string_view>
#include "Server.hpp"
#include "Channel.hpp"

class Client {

	private:
		std::string	_username;
		std::string	_nickname;
		std::string	_hostname;
		std::string	_password;
		std::string _buffer; // to store client input data and append to it
		int			_fd;
		int			_socket;

	public:
		Client() = delete;
		Client(const Client&) = delete;
		Client(Server& server, int socket);
		~Client() = default;

		Client&		operator=(const Client&) = delete;

		const	std::string&	getUsername() const;
		const	std::string&	getNickname() const;
		const	std::string&	getHostname() const;
		int		getFd()	const;

		void	setFd(int fd);
		void	setUsername(std::string_view username);
		void	setNickname(std::string_view nickname);
		void	setHostname(std::string_view hostname);

		void	joinChannel();
		void	leaveChannel();

};