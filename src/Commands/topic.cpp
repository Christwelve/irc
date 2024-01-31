#include "Command.hpp"
#include "UserManager.hpp"
#include "ChannelManager.hpp"
#include "MessageDefines.hpp"

std::string Command::topic(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();

	const std::string &channelName = message.getParamAt(0);

	if(!user.isRegistered())
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "TOPIC"));
	if(!message.hasTrailing())
		return (ERR_TRAILING_MISSING(user, "TOPIC"));
	if(!channelManager.hasChannelWithName(channelName))
		return (ERR_CHANNEL_DOESNT_EXIST(user, channelName));

	Channel &channel = channelManager.getChannelByName(channelName);

	if(!channel.hasUser(user))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, channelName));
	if(channel.isTopicRestricted() && !channel.isUserOp(user))
		return (ERR_USER_NOT_OPERATOR(user, channelName));

	channel.sendMessage(TOPIC_CHANGED(user, channelName, message.getTrailing()));

	return ("");
}
