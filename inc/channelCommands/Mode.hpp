#pragma once

#include "IChannelCommand.hpp"

/* Command: MODE
 * Parameters: <target> [<modestring> [<mode arguments>...]]
 */
class Mode : public virtual IChannelCommand {
	private:

	public:
		Mode(void) = delete;
		Mode(const Mode&) = delete;
		~Mode(void) = default;

		Mode&	operator=(const Mode&) = delete;

		void	execute() override;
};