#pragma once

#include "AChannelCommand.hpp"

/* Command: PART
 * Parameters: <channel>{,<channel>} [<reason>]
 */
class Part : public virtual AChannelCommand {
	private:

	public:
		Part(void) = default;
		Part(const Part&) = delete;
		~Part(void) = default;

		Part&	operator=(const Part&) = delete;

		void	execute(const Client& client,
			const std::vector<std::string_view>& params) override;
};