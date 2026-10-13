#pragma once

#include "AChannelCommand.hpp"

/* Command: INVITE
 * Parameters: <nickname> <channel>
 */
class Invite : public virtual AChannelCommand {
	private:

	public:
		Invite(void) = default;
		Invite(const Invite&) = delete;
		~Invite(void) = default;

		Invite&	operator=(const Invite&) = delete;

		void	execute(Client& client) override;
};