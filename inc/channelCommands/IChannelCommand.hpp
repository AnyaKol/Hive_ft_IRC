#pragma once

class IChannelCommand {
	public:
		virtual ~IChannelCommand(void) = default;

		virtual void	execute() = 0;
};