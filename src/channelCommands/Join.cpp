#include "Join.hpp"
#include "Channel.hpp"

#include <iostream>

Join::Join(Client& client, std::unordered_set<Channel*>& channels)
: AChannelCommand(client, channels) {}

/* Command: JOIN
 * Parameters: <channel>{,<channel>} [<key>{,<key>}]
 * Alt Params: 0
 */
 // Servers may validate the <key> value, eg. to forbid spaces; can forbid a <key> to start from channel name prefix ('#' or '&')
/*
	JOIN #channel1,#channel2,#channel3 key2,key3
	After parsing:
	[#channel1,#channel2,#channel3] [key2,key3,x]
*/
void	Join::execute(const std::vector<std::string_view>& params) {

	//Check if no parameters given
	if (params.size() != 1 || params.size() != 2)
		throw (Channel::Exception(Channel::ERR_WRONG_NUM_PARAM));

	//Print all params
	// int i = 0;
	// for (std::string_view param : params) {
	// 	std::cout << "Param " << i << ": " << param << std::endl;
	// 	i++;
	// }

	// Split <channels> and <keys>
	this->channels = AChannelCommand::splitByCommas(params[0]);
	if (params.size() == 2)
		this->keys = AChannelCommand::splitByCommas(params[1]);

	// Go through <channels> and add executing client; if <key> = "x" then skip it
	try {
		if (this->keys.size() > 0) {
			for (std::string_view channelName : this->channels) {
				Channel* channel = AChannelCommand::findChannel(this->AChannelCommand::allChannels, channelName);
				std::vector<std::string_view>::iterator nextKey = this->keys.begin();

				if (*nextKey != "x")
					channel->addMember(this->AChannelCommand::executor, *nextKey);
				else
					channel->addMember(this->AChannelCommand::executor);
				if (nextKey < this->keys.end())
					nextKey++;
			}
		} else {
			for (std::string_view channelName : this->channels) {
				Channel* channel = AChannelCommand::findChannel(this->AChannelCommand::allChannels, channelName);
				
				channel->addMember(this->AChannelCommand::executor);
			}
		}
	} catch (std::exception &e) {//Need to specify exceptions of class Channel
		this->AChannelCommand::executor.appendToWriteBuffer(e.what());//Send error message to executor client
	}
}