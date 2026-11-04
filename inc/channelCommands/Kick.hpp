#pragma once

#include "AChannelCommand.hpp"

/* Command: KICK
 * Parameters: <channel> <user> *( "," <user> ) [<comment>]
 */
class Kick : public virtual AChannelCommand {
	private:
		Kick(void) = delete;

	public:
		Kick(Client& client, std::unordered_set<Channel*>& channels);
		Kick(const Kick&) = delete;
		~Kick(void) {};

		Kick&	operator=(const Kick&) = delete;

		void	execute(const std::vector<std::string_view>& params) override;
};