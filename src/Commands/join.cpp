#include <iostream>
#include "Command.hpp"
#include "UserManager.hpp"
#include "ChannelManager.hpp"
#include "MessageDefines.hpp"


std::string Command::join(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();

	if(!user.isRegistered())
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "JOIN"));

	const std::string &channelName = message.getParamAt(0);

	if(channelManager.hasChannelWithName(channelName))
	{
		Channel &channel = channelManager.getChannelByName(channelName);

		if(channel.hasUser(user))
			return (ERR_USER_ALREADY_MEMBER_OF_CHANNEL(user, channelName));
		if(channel.isUserLimit() && channel.isFull())
			return (ERR_CHANNEL_IS_FULL(user, channelName));
		if(channel.isInviteOnly() && !channel.isUserInvited(user))
			return (ERR_CHANNEL_INVITE_ONLY(user, channelName));
		if(channel.isKeyRequired() && !channel.isKeyValid(message.getParamAt(1)))
			return (ERR_CHANNEL_INVALID_KEY(user, channelName));

		channel.addUser(user);
		channel.removeInvite(user);

		std::vector<User> &users = channel.getUsers();

		for(unsigned long i = 0; i < users.size(); i++)
		{
			User &target = UserManager::getInstance().getUserByNickname(users.at(i).getNickname());

			target.queue(JOIN_CHANNEL(user, channelName));
		}

		user.queue(JOIN_CHANNEL_SEND_LIST(user, channelName, channel.getUserList()));
		user.queue(JOIN_CHANNEL_SEND_LIST_END(user, channelName));
	}
	else
	{
		if(!ChannelManager::isValidChannelName(channelName))
			return (ERR_CHANNEL_INVALID_NAME(user, channelName));
		Channel &channel = channelManager.createChannelWithName(channelName);

		channel.addUser(user);
		channel.addOperator(user);

		user.queue(JOIN_CHANNEL(user, channelName));
		user.queue(MODE_SET_OPERATOR(user, channelName, "+o", user.getNickname()));
	}

	return ("");
}
