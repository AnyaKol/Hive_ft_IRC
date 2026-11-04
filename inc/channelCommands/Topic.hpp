#pragma once

#include "AChannelCommand.hpp"

/* Command: TOPIC
 * Parameters: <channel> [<topic>]
 */
class Topic : public virtual AChannelCommand {
	private:
		Topic(void) = delete;

	public:
		Topic(Client& client, std::unordered_set<Channel*>& channels);
		Topic(const Topic&) = delete;
		~Topic(void) {};

		Topic&	operator=(const Topic&) = delete;

		void	execute(const std::vector<std::string_view>& params) override;
};