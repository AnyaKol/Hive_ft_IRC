#pragma once

#include "AChannelCommand.hpp"

/* Command: MODE
 * Parameters: <target> [<modestring> [<mode arguments>...]]
 */
class Mode : public virtual AChannelCommand {
	private:
		Mode(void) = delete;

	public:
		Mode(Client& client, std::unordered_set<Channel*>& channels);
		Mode(const Mode&) = delete;
		~Mode(void) {};

		Mode&	operator=(const Mode&) = delete;

		void	execute(const std::vector<std::string_view>& params) override;
};