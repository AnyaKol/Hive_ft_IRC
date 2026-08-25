#pragma once

#include <string>
#include <string_view>

class Client;

/* Channel name may not contain any spaces (' ', 0x20), a control G / BELL
 * ('^G', 0x07), or a comma (',', 0x2C)
 * Channel name prefix may be:
 * 	('#', 0x23) - regular channel; known to all servers that are connected to
 * 	the network
 * 	('&', 0x26) - local channels; the clients connected can only see and talk
 * 	to other clients on the same server
 */
class Channel{

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

		Channel&		operator=(const Channel&) = delete;

		typedef std::set<Client*>::iterator	iterator;

		void			addMember(Client& client);
		void			addOperator(Client& client);
		void			removeMember(Client& client);
		void			removeOperator(Client& client);
		void			memberToOperator(Client& client);
		void			operatorToMember(Client& client);
		void			changeMode(char mode, bool value);

		bool			isFull(void) const;

		std::string&	getName(void) const;
		std::string&	getTopic(void) const;
		std::size_t		getUserLimit(void) const;

		void	setName(std::string_view name);
		void	setTopic(std::string_view topic);
		void	setUserLimit(std::size_t members);

};