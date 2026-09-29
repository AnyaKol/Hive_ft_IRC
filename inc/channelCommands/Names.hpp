#pragma once

#include "IChannelCommand.hpp"

/* Command: NAMES
 * Parameters: <channel>{,<channel>}
 */
class Names : public virtual IChannelCommand {
	private:

	public:
		Names(void) = delete;
		Names(const Names&) = delete;
		~Names(void) = default;

		Names&	operator=(const Names&) = delete;

		void	execute() override;
};