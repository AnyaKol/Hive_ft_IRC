#pragma once

#include "AChannelCommand.hpp"

/* Command: NAMES
 * Parameters: <channel>{,<channel>}
 */
class Names : public virtual AChannelCommand {
	private:

	public:
		Names(void) = delete;
		Names(const Names&) = delete;
		~Names(void) = default;

		Names&	operator=(const Names&) = delete;

		void	execute() override;
};