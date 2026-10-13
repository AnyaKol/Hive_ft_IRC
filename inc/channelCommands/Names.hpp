#pragma once

#include "AChannelCommand.hpp"

/* Command: NAMES
 * Parameters: <channel>{,<channel>}
 */
class Names : public virtual AChannelCommand {
	private:

	public:
		Names(void) = default;
		Names(const Names&) = delete;
		~Names(void) = default;

		Names&	operator=(const Names&) = delete;

		void	execute(Client& client) override;
};