#include "Command.hpp"
#include "UserManager.hpp"
#include "ChannelManager.hpp"
#include "MessageDefines.hpp"

std::string Command::kick(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();
	UserManager &userManager = UserManager::getInstance();

	const std::string &channelName = message.getParamAt(0);
	const std::string &targetName = message.getParamAt(1);

	if(!user.isRegistered())
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 2)
		return (ERR_NEED_MORE_PARAMS(user, "KICK"));
	if(!channelManager.hasChannelWithName(channelName))
		return (ERR_CHANNEL_DOESNT_EXIST(user, channelName));

	Channel &channel = channelManager.getChannelByName(channelName);

	if(!channel.hasUser(user))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, targetName));
	if(!channel.isUserOp(user))
		return (ERR_USER_NOT_OPERATOR(user, channelName));

	if(!userManager.hasUserWithNickname(targetName))
		return (ERR_USER_DOESNT_EXIST(user, targetName));

	User &target = userManager.getUserByNickname(targetName);

	if(!channel.hasUser(target))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, targetName));

	channel.sendMessage(KICK_USER(user, channelName, targetName, message.getTrailing()));
	channel.removeUser(target);

	return ("");
}
