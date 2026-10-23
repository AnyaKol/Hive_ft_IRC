#include "Join.hpp"

/* Command: JOIN
 * Parameters: <channel>{,<channel>} [<key>{,<key>}]
 * Alt Params: 0
 */
void	Join::execute(std::unordered_set<Channel*>& channels,
	const Client& client, const std::vector<std::string_view>& params) {

	(void) channels;
	(void) client;
	(void) params;

	/*
	JOIN #channel1,#channel2,#channel3 key2,key3
	After parsing:
	[#channel1] [#channel2] [#channel3] [key2] [key3]
	*/
}