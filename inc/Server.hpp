#pragma	once

#include <csignal>
#include <cstdint>
#include <fstream>
#include <string>
#include <string_view>

#include <memory>
#include <unordered_map>
#include <vector>

class Server {

	private:
		int		_serverSocket{};
		std::uint16_t	_port{};
		std::string		_password;
		std::string		_hostname;


	public:
		Server(std::uint16_t port, const std::string &password);
		Server() = delete;
		Server(const Server&) = delete;
		~Server() = default;

		Server&		operator=(const Server&) = delete;

		bool		initServer();
		bool		start();

		const	std::uint16_t	getPort();
		const	std::string&	getPassword();
		const	std::string&	getHostname();

};
