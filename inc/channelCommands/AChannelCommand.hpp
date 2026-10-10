#pragma once

#include "Client.hpp"

class AChannelCommand {
	public:
		virtual ~AChannelCommand(void) = default;

		virtual void	execute(Client& client) = 0;
};