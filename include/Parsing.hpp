#ifndef PARSING_HPP
# define PARSING_HPP

# include <iostream>
# include <map>
# include <string>
# include <sstream>
# include <algorithm>
# include "User.hpp"
# include "Socket.hpp"
# include "UserManager.hpp"
# include "Message.hpp"
# include "MessageDefines.hpp"
# include "ChannelManager.hpp"


class Server;
// class ChannelManager;
// class UserManager;

enum Mode
{
	MODE_INVITE,
	MODE_TOPIC,
	MODE_KEY,
	MODE_OP,
	MODE_LIMIT,
	MODE_INVALID
};

enum ModeType
{
	MODE_TYPE_SET,
	MODE_TYPE_UNSET,
	MODE_TYPE_INVALID
};

std::string pass(User &user, const Message &message)
{
	if(user.isRegistered())
		return (ERR_USER_ALREADY_REGISTERED(user));
	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "PASS"));
	if(!user.hasState(USER_EXPECTS_PASS))
		return (ERR_UNKNOWN_COMMAND(user, "PASS"));
	if(!Server::getInstance().isPasswordValid(message.getParamAt(0)))
	{
		UserManager::getInstance().removeUser(user);
		return ("");
	}

	user.setState(USER_EXPECTS_NICK);

	return ("");
}

std::string ping(User &user, const Message &message)
{
	if (message.getParamCount() == 1)
		return (PONG_WITH_PARAM(message));
	if (message.hasTrailing())
		return (PONG_WITH_TRAILING(message));

	return (ERR_NEED_MORE_PARAMS(user, "PING"));
}

std::string cap(User &user, const Message &message)
{
	(void)user;
	(void)message;

	return ("");
}

std::string quit(User &user, const Message &message)
{
	(void)user;
	// user.remove();
	std::cout << "quit() " << user.getNickname() << message.getParamAt(0) << std::endl;
	//sendFuntion()
	// :nickname!username@hostname QUIT :<quit message>\r\n

	return ("");
}

std::string nick(User &user, const Message &message)
{
	//TODO: insert invalid characters in nickname

	UserManager &userManager = UserManager::getInstance();

	if (message.getParamCount() == 0)
		return (ERR_NEED_MORE_PARAMS(user, "NICK"));
	if (userManager.hasUserWithNickname(message.getParamAt(0)))
		return (ERR_NICK_ALREADY_IN_USE(user, message.getParamAt(0)));
	if (user.isRegistered())
	{
		user.setNickname(message.getParamAt(0));
		return (NICK_CHANGED(user, message.getParamAt(0)));
	}
	if (!user.hasState(USER_EXPECTS_NICK))
		return (ERR_UNKNOWN_COMMAND(user, "NICK"));

	// TODO: check if nickname is valid
	user.setNickname(message.getParamAt(0));
	user.setState(USER_EXPECTS_USER);

	return ("");
}

std::string userCmd(User &user, const Message &message)
{
	if (user.isRegistered())
		return (ERR_USER_ALREADY_REGISTERED(user));
	if (message.getParamCount() < 3)
		return (ERR_NEED_MORE_PARAMS(user, "USER"));
	if (!user.hasState(USER_EXPECTS_USER))
		return (ERR_UNKNOWN_COMMAND(user, "USER"));

	user.setUsername(message.getParamAt(0), message.getParamAt(2));
	user.setState(USER_REGISTERED);

	return (USER_WELCOME(user));
}

std::string join(User &user, const Message &message)
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
			return (ERR_USER_IS_ALREADY_ON_CHANNEL(user, channelName));
		if(channel.isFull())
			return (ERR_CHANNEL_IS_FULL(user, channelName));
		if(channel.isInviteOnly() && !channel.isUserInvited(user))
			return (ERR_CHANNEL_INVITE_ONLY(user, channelName));
		if(channel.isKeyRequired() && channel.isKeyValid(message.getParamAt(1)))
			return (ERR_CHANNEL_INVALID_KEY(user, channelName));

		channel.addUser(user);
		channel.removeInvite(user);
		channel.sendMessage(JOIN_CHANNEL(user, channelName));

		return ("");
	}
	else
	{
		if(!ChannelManager::isValidChannelName(channelName))
			return (ERR_CHANNEL_INVALID_NAME(user, channelName));
		Channel &channel = channelManager.createChannelWithName(channelName);
		channel.addUser(user);
		channel.addOperator(user);

		user.queue(JOIN_CHANNEL(user, channelName));
		return (MODE_SET_OPERATOR(user, channelName, "+o", user.getNickname()));
	}

	return ("");
}

std::string part(User &user, const Message &message)
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

	if(channel.isEmpty())
		channelManager.removeChannel(channel);

	return ("");
}

std::string privmsg(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();
	UserManager &userManager = UserManager::getInstance();

	if(!user.isRegistered())
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "PRIVMSG"));

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

		target.queue(PRIVMSG_SEND_MESSAGE(target, target.getNickname(), message.getTrailing()));
	}

	return ("");
}

std::string kick(User &user, const Message &message)
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

	channel.removeUser(target);
	channel.sendMessage(KICK_USER(user, channelName, targetName, message.getTrailing()));

	return ("");
}

std::string invite(User &user, const Message &message)
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

	channel.addInvite(target);

	return (INVITE_USER(user, targetName, channelName));
}

std::string topic(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();

	const std::string &channelName = message.getParamAt(0);

	if(!user.isRegistered())
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "TOPIC"));
	if(!channelManager.hasChannelWithName(channelName))
		return (ERR_CHANNEL_DOESNT_EXIST(user, channelName));

	Channel &channel = channelManager.getChannelByName(channelName);

	if(!channel.hasUser(user))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, channelName));
	if(channel.isTopicRestricted() && !channel.isUserOp(user))
		return (ERR_USER_NOT_OPERATOR(user, channelName));

	return (TOPIC_CHANGED(user, channelName, message.getTrailing()));
}

std::string list(User &user, const Message &message)
{
	(void)user;
	(void)message;
	std::cout << "list() " << std::endl;

	return ("");
}

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

std::string mode(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();

	if(!user.isRegistered())
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 2)
		return (ERR_NEED_MORE_PARAMS(user, "MODE"));

	const std::string &channelName = message.getParamAt(0);

	if(!channelManager.hasChannelWithName(channelName))
		return (ERR_CHANNEL_DOESNT_EXIST(user, channelName));

	Channel &channel = channelManager.getChannelByName(channelName);

	if(!channel.hasUser(user))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, channelName));
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
			return (MODE_SET_KEY_REQUIRED(user, channelName, modeStr, message.getParamAt(2)));
		}
		else
		{
			channel.setKey("");
			return (MODE_UNSET_KEY_REQUIRED(user, channelName, modeStr));
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
			return (MODE_SET_OPERATOR(user, channelName, modeStr, targetName));
		}
		else
		{
			if(target == user)
				return (ERR_INVALID_PARAM(user, targetName));
			if(!channel.isUserOp(target))
				return (ERR_USER_NOT_OPERATOR(user, channelName));
			channel.removeOperator(target);
			return (MODE_UNSET_OPERATOR(user, channelName, modeStr, targetName));
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
			return (MODE_SET_USER_LIMIT(user, channelName, modeStr, message.getParamAt(2)));
		}
		else
		{
			channel.setLimit(0);
			return (MODE_UNSET_USER_LIMIT(user, channelName, modeStr));
		}
	}

	return ("");
}


typedef std::string (*CommandFunc)(User&, const Message&);
std::map<std::string, CommandFunc> getCommandMap()
{
	std::map<std::string, CommandFunc> map;

	map["PASS"] = pass;
	map["PING"] = ping;
	map["CAP" ] = cap;
	map["QUIT"] = quit;
	map["NICK"] = nick;
	map["USER"] = userCmd;
	map["JOIN"] = join;
	map["PART"] = part;
	map["PRIVMSG"] = privmsg;
	map["KICK"] = kick;
	map["INVITE"] = invite;
	map["TOPIC"] = topic;
	map["LIST"] = list;
	map["MODE"] = mode;

	return (map);
}

void parseInput(User &user, std::string input)
{
	static std::map<std::string, CommandFunc> commandMap = getCommandMap();

	Message message(input);

	if(user.hasState(USER_EXPECTS_PASS) && message.getCommand() != "PASS")
	{
		// TODO: send error message
		user.remove();
		return;
	}

    std::map<std::string, CommandFunc>::iterator it = commandMap.find(message.getCommand());

    if (it != commandMap.end())
	{
		std::string response = it->second(user, message);
		if (!response.empty())
			user.queue(response);
	}
    else
        user.queue(ERR_UNKNOWN_COMMAND(user, message.getCommand()));
}

#endif
