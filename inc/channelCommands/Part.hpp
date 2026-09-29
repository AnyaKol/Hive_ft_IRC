#pragma once

#include "IChannelCommand.hpp"

/* Command: PART
 * Parameters: <channel>{,<channel>} [<reason>]
 */
class Part : public virtual IChannelCommand {
	private:

	public:
		Part(void) = delete;
		Part(const Part&) = delete;
		~Part(void) = default;

		Part&	operator=(const Part&) = delete;

		void	execute() override;
};