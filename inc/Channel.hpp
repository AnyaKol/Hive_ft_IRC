#pragma once

#include <string>
#include <string_view>

class Channel{

	private:
		std::string		_name;
		std::string		_topic;
		std::size_t		maxMembers{};
		bool			inviteOnly = false;


	public:
		Channel(std::string_view name);
		Channel(const Channel&) = delete;
		~Channel() = default;

		Channel&		operator=(const Channel&) = delete;

		void			addMember();
		void			removeMember();
		void			addMode();
		void			removeMode();
		bool			ifFull() const;

		std::size_t		getMembers() const;

};