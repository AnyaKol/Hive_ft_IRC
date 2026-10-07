#pragma once

#include "AChannelCommand.hpp"

/* Command: TOPIC
 * Parameters: <channel> [<topic>]
 */
class Topic : public virtual AChannelCommand {
	private:

	public:
		Topic(void) = delete;
		Topic(const Topic&) = delete;
		~Topic(void) = default;

		Topic&	operator=(const Topic&) = delete;

		void	execute() override;
};