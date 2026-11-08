#pragma once

#include "AChannelCommand.hpp"

/* Command: PART
 * Parameters: <channel>{,<channel>} [<reason>]
 */
class Part : public virtual AChannelCommand {
	private:
		Part(void) = delete;

	public:
		Part(Client& client, std::unordered_map<std::string, Channel>& channels);
		Part(const Part&) = delete;
		~Part(void) {};

		Part&	operator=(const Part&) = delete;

		void	execute(const std::vector<std::string_view>& params) override;
};