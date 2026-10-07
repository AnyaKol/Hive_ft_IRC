#pragma once

class AChannelCommand {
	public:
		virtual ~AChannelCommand(void) = default;

		virtual void	execute() = 0;
};