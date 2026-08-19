#pragma once

#include <string>
#include <string_view>

class Channel{

	private:
		std::string		_name;
		std::string		_topic;
		std::size_t		_maxMembers{};
		std::vector<int> _memfds;
		bool			inviteOnly = false;


	public:
		Channel(std::string_view name);
		Channel(const Channel&) = delete;
		~Channel(void) = default;

		Channel&		operator=(const Channel&) = delete;

		void			addMember(void);
		void			removeMember(void);
		void			addMode(void);
		void			removeMode(void);
		bool			ifFull(void) const;

		std::string&	getName(void) const;
		std::string&	getTopic(void) const;
		std::size_t		getMaxMembers(void) const;

};