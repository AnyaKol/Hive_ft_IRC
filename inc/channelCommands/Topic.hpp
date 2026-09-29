#pragma once

#include "IChannelCommand.hpp"

/* Command: TOPIC
 * Parameters: <channel> [<topic>]
 */
class Topic : public virtual IChannelCommand {
	private:

	public:
		Topic(void) = delete;
		Topic(const Topic&) = delete;
		~Topic(void) = default;

		Topic&	operator=(const Topic&) = delete;

		void	execute() override;
};