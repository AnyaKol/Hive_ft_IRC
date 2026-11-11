#pragma once

//Commands
#include "IRC.hpp"
#include "AChannelCommand.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

class Server;
class Client;

class Channel {

	private:
		std::string			_name;
		std::string			_topic;
		std::string			_key;
		std::size_t			_userLimit;

		std::unordered_set<Client*>		_operators {};
		std::unordered_set<Client*>		_members {};
		std::unordered_set<std::string>	_pendingInvites {};

		bool	_inviteOnly = false;
		bool	_protectedTopic = false;
		bool	_hasKey = false;
		bool	_hasUserLimit = false;

	public:
		Channel(void) = delete;
		Channel(std::string_view name, Client& creator);
		Channel(const Channel&) = delete;
		~Channel(void) = default;
		Channel(Channel&& other) noexcept = default;

		Channel&	operator=(const Channel&) = delete;

		typedef std::unordered_set<Client*>::iterator	iterator;

		void	addMember(Client& client, std::string_view key = "");
		void	addOperator(Client& client);
		void	removeMember(Client& client);
		void	removeOperator(Client& client);
		bool	removeClientEverywhere(Client& client, const std::string& reason);
		void	memberToOperator(Client& client);
		void	operatorToMember(Client& client);

		bool	isFull(void) const;
		bool	isMember(Client& client) const;
		bool	isOperator(Client& client) const;
		void	publicMessage(const std::string& msg, const Client* sender = nullptr) const;
		bool	invite(const std::string& nickname);

		// Numeric codes
		static void	sendNumeric(Client& client, IRC::NumericCodes code,
			const std::string& params = "", const std::string& trailing = "");

		// Getters
		const std::string&	getName(void) const { return (this->_name); };
		const std::string&	getTopic(void) const { return (this->_topic); };
		const std::size_t&	getUserLimit(void) const { return (this->_userLimit); };
		const std::string&	getKey(void) const { return (this->_key); };

		const std::unordered_set<Client*>&	getOperators(void) const { return (this->_operators); };
		const std::unordered_set<Client*>&	getMembers(void) const { return (this->_members); };

		bool	isInviteOnly(void) const { return (this->_inviteOnly); };
		bool	isTopicProtected(void) const { return (this->_protectedTopic); };
		bool	hasKey(void) const { return (this->_hasKey); };
		bool	hasUserLimit(void) const { return (this->_hasUserLimit); };

		Client*	getClientByNick(const std::string_view& nick) const;

		// Setters
		void	setName(std::string_view name);
		void	setKey(std::string key, Client& client) ;
		void	clearKey(void);
		void	setInviteOnly(bool value);
		void	setTopicProtected(bool value);
		void	setTopic(std::string_view topic, Client& client);
		void	setUserLimit(std::size_t members);
		void	clearUserLimit(void);
		static bool	isValidChannelName(std::string_view name);

		// Commands
		static bool				isChannelCommand(Server& server,
			std::unordered_map<std::string, Channel>& channels,
			Client& client, const std::string& command,
			const std::vector<std::string_view>& params);
		static AChannelCommand*	makeCommand(Server& server, const std::string& command,
			Client& client, std::unordered_map<std::string, Channel>& channels);
		static int				makeCommandNumber(const std::string& command);

		// Exception messages
		static const std::string	ERR_NOT_CHANNEL_COMMAND;
		static const std::string	ERR_WRONG_NUM_PARAM;

		// NameException messages
		static const std::string	ERR_CREATE_CHANNEL;

		class	Exception;
		class	NameException;
};

// Exceptions
class	Channel::Exception : public std::exception {
	private:
		Exception(void) = delete;
		std::string	_msg;
	public:
		Exception(const std::string& message);
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