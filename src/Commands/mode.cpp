#include "Command.hpp"
#include "UserManager.hpp"
#include "ChannelManager.hpp"
#include "MessageDefines.hpp"


Mode getModeFromString(const std::string &str)
{
	if(str.at(1) == 'i')
		return (MODE_INVITE);
	if(str.at(1) == 't')
		return (MODE_TOPIC);
	if(str.at(1) == 'k')
		return (MODE_KEY);
	if(str.at(1) == 'o')
		return (MODE_OP);
	if(str.at(1) == 'l')
		return (MODE_LIMIT);

	return (MODE_INVALID);
}

ModeType getModeTypeFromString(const std::string &str)
{
	if(str.at(0) == '+')
		return (MODE_TYPE_SET);
	if(str.at(0) == '-')
		return (MODE_TYPE_UNSET);

	return (MODE_TYPE_INVALID);
}

std::string Command::mode(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();

	if(!user.isRegistered())
		return (ERR_USER_NOT_REGISTERED(user));

	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "MODE"));


	const std::string &channelName = message.getParamAt(0);

	if(!channelManager.hasChannelWithName(channelName))
		return (ERR_CHANNEL_DOESNT_EXIST(user, channelName));

	Channel &channel = channelManager.getChannelByName(channelName);

	if(!channel.hasUser(user))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, channelName));

	if(message.getParamCount() == 1)
	{
		std::string activeModes = "+";

		if(channel.isInviteOnly())
			activeModes += "i";
		if(channel.isTopicRestricted())
			activeModes += "t";
		if(channel.isKeyRequired())
			activeModes += "k";
		if(channel.isUserOp(user))
			activeModes += "o";
		if(channel.isUserLimit())
			activeModes += "l";

		return (MODE_CHANNEL_INFO(user, channelName, activeModes));
	}

	if(message.getParamCount() < 2)
		return (ERR_NEED_MORE_PARAMS(user, "MODE"));

	if(!channel.isUserOp(user))
		return (ERR_USER_NOT_OPERATOR(user, channelName));

	const std::string &modeStr = message.getParamAt(1);

	if(modeStr.length() != 2)
		return (ERR_INVALID_PARAM(user, modeStr));

	Mode mode = getModeFromString(modeStr);
	ModeType modeType = getModeTypeFromString(modeStr);

	if(mode == MODE_INVALID || modeType == MODE_TYPE_INVALID)
		return (ERR_INVALID_PARAM(user, modeStr));

	if(mode == MODE_INVITE)
	{
		channel.setInviteOnly(modeType == MODE_TYPE_SET);
		return (MODE_INVITE_ONLY(user, channelName, modeStr));
	}
	if(mode == MODE_TOPIC)
	{
		channel.setTopicRestricted(modeType == MODE_TYPE_SET);
		return (MODE_TOPIC_RESTRICTED(user, channelName, modeStr));
	}
	if(mode == MODE_KEY)
	{
		if(modeType == MODE_TYPE_SET && message.getParamCount() < 3)
			return (ERR_NEED_MORE_PARAMS(user, "MODE"));

		channel.setKeyRequired(modeType == MODE_TYPE_SET);

		if(modeType == MODE_TYPE_SET)
		{
			channel.setKey(message.getParamAt(2));
			channel.sendMessage(MODE_SET_KEY_REQUIRED(user, channelName, modeStr, message.getParamAt(2)));

			return ("");
		}
		else
		{
			channel.setKey("");
			channel.sendMessage(MODE_UNSET_KEY_REQUIRED(user, channelName, modeStr));

			return ("");
		}
	}
	if(mode == MODE_OP)
	{
		if(message.getParamCount() < 3)
			return (ERR_NEED_MORE_PARAMS(user, "MODE"));

		UserManager &userManager = UserManager::getInstance();
		const std::string &targetName = message.getParamAt(2);

		if(!userManager.hasUserWithNickname(targetName))
			return (ERR_USER_DOESNT_EXIST(user, targetName));

		User &target = userManager.getUserByNickname(targetName);

		if(!channel.hasUser(target))
			return (ERR_NOT_MEMBER_OF_CHANNEL(user, channelName));

		if(modeType == MODE_TYPE_SET)
		{
			if(channel.isUserOp(target))
				return (ERR_USER_ALREADY_OPERATOR(user, channelName, targetName));

			channel.addOperator(target);
			channel.sendMessage(MODE_SET_OPERATOR(user, channelName, modeStr, targetName));

			return ("");
		}
		else
		{
			if(target == user)
				return (ERR_INVALID_PARAM(user, targetName));
			if(!channel.isUserOp(target))
				return (ERR_TARGET_NOT_OPERATOR(targetName, channelName));

			channel.removeOperator(target);
			channel.sendMessage(MODE_UNSET_OPERATOR(user, channelName, modeStr, targetName));

			return ("");
		}
	}
	if(mode == MODE_LIMIT)
	{
		if(modeType == MODE_TYPE_SET && message.getParamCount() < 3)
			return (ERR_NEED_MORE_PARAMS(user, "MODE"));

		channel.setUserLimit(modeType == MODE_TYPE_SET);

		if(modeType == MODE_TYPE_SET)
		{
			int limit = std::atoi(message.getParamAt(2).c_str());

			if(limit < 1)
				return (ERR_INVALID_PARAM(user, message.getParamAt(2)));

			channel.setLimit(static_cast<unsigned int>(limit));
			channel.sendMessage(MODE_SET_USER_LIMIT(user, channelName, modeStr, message.getParamAt(2)));

			return ("");
		}
		else
		{
			channel.setLimit(0);
			channel.sendMessage(MODE_UNSET_USER_LIMIT(user, channelName, modeStr));

			return ("");
		}
	}

	return ("");
}
