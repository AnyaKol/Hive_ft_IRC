#pragma once

#include "AChannelCommand.hpp"

/* Command: JOIN
 * Parameters: <channel>{,<channel>} [<key>{,<key>}]
 * Alt Params: 0
 */
class Join : public virtual AChannelCommand {
	private:

	public:
		Join(void) = default;
		Join(const Join&) = delete;
		~Join(void) = default;

		Join&	operator=(const Join&) = delete;

		void	execute(Client& client) override;
};