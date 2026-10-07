#pragma once

#include "AChannelCommand.hpp"

/* Command: MODE
 * Parameters: <target> [<modestring> [<mode arguments>...]]
 */
class Mode : public virtual AChannelCommand {
	private:

	public:
		Mode(void) = delete;
		Mode(const Mode&) = delete;
		~Mode(void) = default;

		Mode&	operator=(const Mode&) = delete;

		void	execute() override;
};