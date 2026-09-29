#pragma once

#include "IChannelCommand.hpp"

/* Command: JOIN
 * Parameters: <channel>{,<channel>} [<key>{,<key>}]
 * Alt Params: 0
 */
class Join : public virtual IChannelCommand {
	private:

	public:
		Join(void) = delete;
		Join(const Join&) = delete;
		~Join(void) = default;

		Join&	operator=(const Join&) = delete;

		void	execute() override;
};