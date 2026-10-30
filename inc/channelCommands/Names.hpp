#pragma once

#include "AChannelCommand.hpp"

/* Command: NAMES
 * Parameters: <channel>{,<channel>}
 */
class Names : public virtual AChannelCommand {
	private:
		Names(void) = delete;

	public:
		Names(Client& client, std::unordered_set<Channel*>& channels);
		Names(const Names&) = delete;
		~Names(void) {};

		Names&	operator=(const Names&) = delete;

		void	execute(const std::vector<std::string_view>& params) override;
};