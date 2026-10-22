#include "Join.hpp"

/* Command: JOIN
 * Parameters: <channel>{,<channel>} [<key>{,<key>}]
 * Alt Params: 0
 */
//TODO: Treat params as lists of channels and keys
void	Join::execute(const std::set<Channel*>& channels, Client& client,
	const std::vector<std::string_view>& params) {

	
}