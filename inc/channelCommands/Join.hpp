#pragma once

#include "AChannelCommand.hpp"

/* Command: JOIN
 * Parameters: <channel>{,<channel>} [<key>{,<key>}]
 * Alt Params: 0
 */
class Join : public virtual AChannelCommand {
	private:
		Join(void) = delete;

		std::vector<std::string_view>	channels;
		std::vector<std::string_view>	keys {};

		void	sendJoinMessage(Channel& channel) const;

	public:
		Join(Client& client, std::unordered_map<std::string, Channel>& channels, Server& server);
		Join(const Join&) = delete;
		~Join(void) {};

		Join&	operator=(const Join&) = delete;

		void	execute(const std::vector<std::string_view>& params) override;
};