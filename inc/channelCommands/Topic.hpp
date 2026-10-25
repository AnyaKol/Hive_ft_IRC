#pragma once

#include "AChannelCommand.hpp"

/* Command: TOPIC
 * Parameters: <channel> [<topic>]
 */
class Topic : public virtual AChannelCommand {
	private:

	public:
		Topic(void) = default;
		Topic(const Topic&) = delete;
		~Topic(void) = default;

		Topic&	operator=(const Topic&) = delete;

		void	execute(std::unordered_set<Channel*>& channels,
			const Client& client, const std::vector<std::string_view>& params) override;
};