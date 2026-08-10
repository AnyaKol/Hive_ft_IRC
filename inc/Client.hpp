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

		void	setUsername(std::string_view username);
		void	setNickname(std::string_view nickname);
		void	setHostname(std::string_view hostname);

		void	joinChannel();
		void	leaveChannel();

};