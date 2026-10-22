#pragma once

//Commands
#include "Invite.hpp"
#include "Join.hpp"
#include "Kick.hpp"
#include "Mode.hpp"
#include "Names.hpp"
#include "Part.hpp"
#include "Topic.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <set>

class Server;
class Client;

class Channel {

	private:
		std::string	_name;
		std::string	_topic;
		std::string	_key;
		std::size_t	_userLimit;

		std::set<Client*>	_operators{};
		std::set<Client*>	_members{};
		std::set<Client*>	_pendingInvites{};

		bool	_inviteOnly = false;
		bool	_protectedTopic = false;
		bool	_hasKey = false;
		bool	_hasUserLimit = false;

	public:
		Channel(void) = delete;
		Channel(std::string_view name, Client& creator);
		Channel(const Channel&) = delete;
		~Channel(void) = default;

		Channel&	operator=(const Channel&) = delete;

		typedef std::set<Client*>::iterator	iterator;

		void	addMember(Client& client);
		void	addOperator(Client& client);
		void	removeMember(Client& client);
		void	removeOperator(Client& client);
		void	memberToOperator(Client& client);
		void	operatorToMember(Client& client);
		void	changeMode(char mode, bool value);

		void	publicMessage(const std::string& msg) const;

		bool	isFull(void) const;

		std::string_view	getName(void) const;
		std::string_view	getTopic(void) const;
		std::size_t			getUserLimit(void) const;

		void	setName(std::string_view name);
		void	setTopic(std::string_view topic, Client& client);
		void	setUserLimit(std::size_t members);

		//Commands
		static bool				isChannelCommand(const std::set<Channel*>& channels,
			const Client& client, const std::string& command,
			const std::vector<std::string_view>& params);
		static AChannelCommand&	makeCommand(const std::string& command);
		static int				makeCommandNumber(const std::string& command);

		//Exceptions
		static const std::string	ERR_CHANNEL_NOT_FOUND;

		class	NotChannelCommandException;
		class	NameException;
};

//Exceptions
class	Channel::NotChannelCommandException : public std::exception {
	private:
		static const std::string	_msg;
	public:
		NotChannelCommandException() = default;
		~NotChannelCommandException() = default;
		const char*	what(void) const noexcept override;
};

class	Channel::NameException : public std::exception {
	private:
		NameException(void) = delete;
		std::string	_msg;
	public:
		NameException(const std::string& message, std::string_view name);
		const char*	what(void) const noexcept override;
};