#pragma once

#include "AChannelCommand.hpp"

/* Command: KICK
 * Parameters: <channel> <user> *( "," <user> ) [<comment>]
 */
class Kick : public virtual AChannelCommand {
	private:

	public:
		Kick(void) = default;
		Kick(const Kick&) = delete;
		~Kick(void) = default;

		Kick&	operator=(const Kick&) = delete;

		void	execute(std::unordered_set<Channel*>& channels,
			const Client& client, const std::vector<std::string_view>& params) override;
};