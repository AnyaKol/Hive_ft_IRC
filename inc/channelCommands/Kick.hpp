#pragma once

#include "IChannelCommand.hpp"

/* Command: KICK
 * Parameters: <channel> <user> *( "," <user> ) [<comment>]
 */
class Kick : public virtual IChannelCommand {
	private:

	public:
		Kick(void) = delete;
		Kick(const Kick&) = delete;
		~Kick(void) = default;

		Kick&	operator=(const Kick&) = delete;

		void	execute() override;
};