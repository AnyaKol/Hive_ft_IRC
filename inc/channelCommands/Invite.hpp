#pragma once

#include "IChannelCommand.hpp"

/* Command: INVITE
 * Parameters: <nickname> <channel>
 */
class Invite : public virtual IChannelCommand {
	private:

	public:
		Invite(void) = delete;
		Invite(const Invite&) = delete;
		~Invite(void) = default;

		Invite&	operator=(const Invite&) = delete;

		void	execute() override;
};