#include "Command.hpp"
#include "UserManager.hpp"
#include "ChannelManager.hpp"
#include "MessageDefines.hpp"

std::string Command::privmsg(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();
	UserManager &userManager = UserManager::getInstance();

	if(!user.isRegistered())
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "PRIVMSG"));
	if(!message.hasTrailing())
		return (ERR_TRAILING_MISSING(user, "PRIVMSG"));

	const std::string &targetName = message.getParamAt(0);

	if(targetName.at(0) == '#')
	{
		if(!channelManager.hasChannelWithName(targetName))
			return (ERR_CHANNEL_DOESNT_EXIST(user, targetName));

		Channel &channel = channelManager.getChannelByName(targetName);

		if(!channel.hasUser(user))
			return (ERR_NOT_MEMBER_OF_CHANNEL(user, targetName));

		channel.broadcastMessage(user, message.getTrailing());
	}
	else
	{
		if(!userManager.hasUserWithNickname(targetName))
			return (ERR_USER_DOESNT_EXIST(user, targetName));

		User &target = userManager.getUserByNickname(targetName);

		target.queue(PRIVMSG_SEND_MESSAGE(user, targetName, message.getTrailing()));
	}

	return ("");
}
