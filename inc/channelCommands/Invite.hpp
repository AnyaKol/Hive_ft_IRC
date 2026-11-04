#pragma once

#include "AChannelCommand.hpp"

/* Command: INVITE
 * Parameters: <nickname> <channel>
 */
class Invite : public virtual AChannelCommand {
	private:
		Invite(void) = delete;

	public:
		Invite(Client& client, std::unordered_set<Channel*>& channels);
		Invite(const Invite&) = delete;
		~Invite(void) {};

		Invite&	operator=(const Invite&) = delete;

		void	execute(const std::vector<std::string_view>& params) override;
};