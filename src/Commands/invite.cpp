#include "Command.hpp"
#include "UserManager.hpp"
#include "ChannelManager.hpp"
#include "MessageDefines.hpp"
#include "Channel.hpp"

std::string Command::invite(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();
	UserManager &userManager = UserManager::getInstance();

	const std::string &targetName = message.getParamAt(0);
	const std::string &channelName = message.getParamAt(1);

	if(!user.isRegistered())
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 2)
		return (ERR_NEED_MORE_PARAMS(user, "INVITE"));
	if(!userManager.hasUserWithNickname(targetName))
		return (ERR_USER_DOESNT_EXIST(user, targetName));
	if(!channelManager.hasChannelWithName(channelName))
		return (ERR_CHANNEL_DOESNT_EXIST(user, channelName));

	Channel &channel = channelManager.getChannelByName(channelName);

	if(!channel.hasUser(user))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, channelName));
	if(!channel.isUserOp(user))
		return (ERR_USER_NOT_OPERATOR(user, channelName));

	User &target = userManager.getUserByNickname(targetName);

	if(channel.isUserInvited(target))
		return (ERR_USER_ALREADT_INVITED_TO_CHANNEL(user, channelName));

	channel.addInvite(target);

	return (INVITE_USER(user, targetName, channelName));
}
