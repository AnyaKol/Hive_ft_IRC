#pragma	once

#include <sys/poll.h>
#include <sys/types.h>
#include <iostream>
#include <csignal>
#include <cstdint>
#include <fstream>
#include <string>
#include <string_view>

#include <memory>
#include <unordered_map> // pair key value dicionary (e.g socket/ client)
#include <vector>
#include "Client.hpp"

#define MAXLINE 1024
class Server {

	private:
		std::uint16_t	_port{};
		std::string		_password;
		int				_serverSocket{};
		std::string		_hostname;
		std::ofstream	_serverLog; // record server events

		static bool	_signal;
		std::unordered_map<int, Client> _clients;
		std::vector<struct pollfd> _pollfds;


	public:
		Server(std::uint16_t port, std::string &password);
		Server() = delete;
		Server(const Server&) = delete;
		~Server() = default;

		Server&		operator=(const Server&) = delete;

		bool		initServer();
		void		runServer();

		std::uint16_t	getPort();
		const	std::string&	getPassword();
		const	std::string&	getHostname();

		void	acceptClient();
		void	receiveData(int fd);
		void	clearClient(int fd);

		static void	signalHandler(int sig);

		void	closeAll();
};
