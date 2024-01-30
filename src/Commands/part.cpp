#include "Command.hpp"
#include "UserManager.hpp"
#include "ChannelManager.hpp"
#include "MessageDefines.hpp"

std::string Command::part(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();

	if(!user.isRegistered())
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "PART"));

	const std::string &channelName = message.getParamAt(0);

	if(!channelManager.hasChannelWithName(channelName))
		return (ERR_CHANNEL_DOESNT_EXIST(user, channelName));

	Channel &channel = channelManager.getChannelByName(channelName);

	if(!channel.hasUser(user))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, channelName));

	channel.removeUser(user);
	channel.sendMessage(PART_CHANNEL(user, channelName, message.getTrailing()));
	user.queue(PART_CHANNEL(user, channelName, message.getTrailing()));

	if(channel.isEmpty())
		channelManager.removeChannel(channel);
	else if (!channel.hasOperator())
	{
		User &newOp = UserManager::getInstance().getUserByNickname(channel.getUsers().at(0).getNickname());

		channel.addOperator(newOp);
		channel.sendMessage(MODE_SET_OPERATOR(user, channelName, "+o", newOp.getNickname()));
	}

	return ("");
}
