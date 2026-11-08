#include "Part.hpp"
#include "Channel.hpp"

Part::Part(Client& client, std::unordered_map<std::string, Channel>& channels)
: AChannelCommand(client, channels) {}

void	Part::execute(const std::vector<std::string_view>& params) {
	
}