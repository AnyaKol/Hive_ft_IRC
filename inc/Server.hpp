#pragma	once

#include <iostream>
#include <csignal>
#include <cstdint>
#include <fstream>
#include <string>
#include <string_view>

#include <memory>
#include <unordered_map> // pair key value dicionary (e.g socket/ client)
#include <vector>

class Server {

	private:
		int		_serverSocket{};
		std::uint16_t	_port{};
		std::string		_password;
		std::string		_hostname;
		std::ofstream	_serverLog; // record server events
		id_t	_pollFd{};


	public:
		Server(std::uint16_t port, const std::string &password);
		Server() = delete;
		Server(const Server&) = delete;
		~Server() = default;

		Server&		operator=(const Server&) = delete;

		void		initServer();
		bool		runServer();

		const	std::uint16_t	getPort();
		const	std::string&	getPassword();
		const	std::string&	getHostname();

};
