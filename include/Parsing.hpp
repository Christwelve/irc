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

// std::string serverToClient(User &user, int status, std::string command) {
//     std::map<int, std::string> statusMap;
//     statusMap[001] = ":" + std::string(SERVER_NAME) + " 001 " + user.getNickname() + " :Welcome message sent after a successful connection.";
//     // statusMap[002] = ":" + std::string(SERVER_NAME) + " 002 " + user.getNickname() + " :Server's version information.";
//     // statusMap[003] = ":" + std::string(SERVER_NAME) + " 003 " + user.getNickname() + " :Server's creation date.";
//     // statusMap[004] = ":" + std::string(SERVER_NAME) + " 004 " + user.getNickname() + " :Server's details (name, version, available user modes, available channel modes).";
//     // statusMap[005] = ":" + std::string(SERVER_NAME) + " 005 " + user.getNickname() + " :Additional server features.";
//     // statusMap[353] = ":" + std::string(SERVER_NAME) + " 353 " + user.getNickname() + " :Reply to the NAMES command, listing all visible users in a channel.";
//     // statusMap[366] = ":" + std::string(SERVER_NAME) + " 366 " + user.getNickname() + " :End of NAMES list.";
//     // statusMap[372] = ":" + std::string(SERVER_NAME) + " 372 " + user.getNickname() + " :MOTD (Message of the Day) text line.";
//     // statusMap[375] = ":" + std::string(SERVER_NAME) + " 375 " + user.getNickname() + " :Start of MOTD.";
//     // statusMap[376] = ":" + std::string(SERVER_NAME) + " 376 " + user.getNickname() + " :End of MOTD.";
//     statusMap[421] = ":" + std::string(SERVER_NAME) + " 421 " + user.getNickname() + " :Unknown command.";
//     statusMap[433] = ":" + std::string(SERVER_NAME) + " 433 " + user.getNickname() + " :Nickname is already in use.";
//     statusMap[441] = ":" + std::string(SERVER_NAME) + " 441 " + user.getNickname() + " :User not in channel (used when trying to kick a user not in channel).";
//     statusMap[442] = ":" + std::string(SERVER_NAME) + " 442 " + user.getNickname() + " :Not on channel (used when trying to perform a channel operation without being on the channel).";
//     statusMap[461] = ":" + std::string(SERVER_NAME) + " 461 " + user.getNickname() + " " + command + " :Need more parameters.";
//     statusMap[462] = ":" + std::string(SERVER_NAME) + " 462 " + user.getNickname() + " :Already registered (used when a client tries to register again).";
//     statusMap[464] = ":" + std::string(SERVER_NAME) + " 464 " + user.getNickname() + " :Incorrect password.";
//     statusMap[465] = ":" + std::string(SERVER_NAME) + " 465 " + user.getNickname() + " :You are banned from this server.";
//     statusMap[471] = ":" + std::string(SERVER_NAME) + " 471 " + user.getNickname() + " :Channel is full.";
//     statusMap[473] = ":" + std::string(SERVER_NAME) + " 473 " + user.getNickname() + " :Invite only channel.";
//     statusMap[474] = ":" + std::string(SERVER_NAME) + " 474 " + user.getNickname() + " :Banned from channel.";
//     statusMap[475] = ":" + std::string(SERVER_NAME) + " 475 " + user.getNickname() + " :Bad channel key.";

//     return statusMap[status];
// }

class Server;

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

	user.queue(":" + std::string(SERVER_NAME) + " 001 " + user.getNickname() + " :Ey Mate! Welcome to 👁️  ®️  🌊");

	return ("");
}

std::string join(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();

	if(!user.isRegistered())
		// TODO: user not registered error
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "JOIN"));

	const std::string &channelName = message.getParamAt(0);

	if(channelManager.hasChannelWithName(channelName))
	{
		Channel &channel = channelManager.getChannelWithName(channelName);
		if(channel.hasUser(user))
		// TODO: what to use here?
			return (ERR_USER_ALREADY_IN_CHANNEL(user, channelName));
		// TODO: use proper error messages
		if(channel.isFull())
			return (ERR_CHANNEL_FULL(user, channelName));
		if(channel.isInviteOnly() && !channel.hasUserWithMode(user, CHANNEL_MODE_O))
			return (ERR_CHANNEL_INVITE_ONLY(user, channelName));
		if(channel.isKeyRequired() && channel.isKeyValid(message.getParamAt(1)))
			return (ERR_CHANNEL_INVALID_KEY(user, channelName));
		channel.addUser(user);
	}
	else
	{
		if(!Channel::isValidChannelName(channelName))
			return (ERR_CHANNEL_INVALID_NAME(user, channelName));
		Channel &channel = channelManager.createChannelWithName(channelName);
		channel.addUser(user);
		channel.addOperator(user);
	}

	return ("");
}

std::string part(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();

	if(!user.isRegistered())
		// TODO: user not registered error
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "PART"));

	const std::string &channelName = message.getParamAt(0);

	if(!channelManager.hasChannelWithName(channelName))
		return (ERR_CHANNEL_DOESNT_EXIST(user, channelName));

	Channel &channel = channelManager.getChannelWithName(channelName);

	if(!channel.hasUser(user))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, channelName));
	channel.removeUser(user);
	channel.sendMessage(user, "User " + user.getNickname() + " has left the channel" + (message.hasTrailing() ? " (" + message.getTrailing() + ")" : ""));
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
	if(message.getParamCount() < 2)
		return (ERR_NEED_MORE_PARAMS(user, "PRIVMSG"));

	const std::string &targetName = message.getParamAt(0);

	if(targetName[0] == '#')
	{
		if(!channelManager.hasChannelWithName(targetName))
			return (ERR_CHANNEL_DOESNT_EXIST(user, targetName));
		Channel &channel = channelManager.getChannelWithName(targetName);
		if(!channel.hasUser(user))
			return (ERR_NOT_MEMBER_OF_CHANNEL(user, targetName));
		channel.sendMessage(user, message.getTrailing());
	}
	else
	{
		if(!userManager.hasUserWithNickname(targetName))
			return (ERR_NICK_DOESNT_EXIST(user, targetName));
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
		return (ERR_CHANNEL_DOESNT_EXIST(user, channelName))

	Channel &channel = channelManager.getChannelByName(channelName);

	if(!channel.hasUser(user))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, targetName));
	if(!channel.isUserOp(user))
		return (ERR_USER_NOT_OPERATOR(user));

	if(userManager.hasUserWithNickname(targetName))
		return (ERR_NICK_DOESNT_EXIST(user, targetName));

	User target& = userManager.getUserByNickname(targetName);

	if(!channel.hasUser(target))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, targetName));

	channel.removeUser(target);
	channel.sendMessage(user, "User " + target.getNickname() + " has been kicked from the channel" + (message.hasTrailing() ? " (" + message.getTrailing() + ")" : ""));

	return ("");
}

std::string invite(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();
	UserManager &userManager = UserManager::getInstance();

	std::string targetName = message.getParamAt(0);
	std::string channelName = message.getParamAt(1);

	if(!user.isRegistered())
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 2)
		return (ERR_NEED_MORE_PARAMS(user, "INVITE"));
	if(!userManager.hasUserWithNickname(targetName))
		return (ERR_NICK_DOESNT_EXIST(user, targetName));
	if(!channelManager.hasChannelWithName(channelName))
		return (ERR_CHANNEL_DOESNT_EXIST(user, channelName));

	Channel &channel = channelManager.getChannelWithName(channelName);

	if(!channel.hasUser(user))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, channelName));

	User &target = userManager.getUserByNickname(targetName);

	return (INVITE_INVITE_USER(user, target, channel));
}

std::string topic(User &user, const Message &message)
{
	ChannelManager &channelManager = ChannelManager::getInstance();

	std::string channelName = message.getParamAt(0);

	if(!user.isRegistered())
		return (ERR_USER_NOT_REGISTERED(user));
	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "TOPIC"));
	if(!channelManager.hasChannelWithName(channelName))
		return (ERR_CHANNEL_DOESNT_EXIST(user, channelName));

	Channel &channel = channelManager.getChannelWithName(channelName);

	if(!channel.hasUser(user))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, channelName));
	if(channel.isTopicRestricted() && !channel.isUserOp(user))
		return (ERR_USER_NOT_OPERATOR(user));

	return (TOPIC_SET_TOPIC(user, channel));
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
	if(str.at(1) == "i")
		return (MODE_INVITE);
	if(str.at(1) == "t")
		return (MODE_TOPIC);
	if(str.at(1) == "k")
		return (MODE_KEY);
	if(str.at(1) == "o")
		return (MODE_OP);
	if(str.at(1) == "l")
		return (MODE_LIMIT);

	return (MODE_INVALID);
}

ModeType getModeTypeFromString(const std::string &str)
{
	if(str.at(0) == "+")
		return (MODE_TYPE_SET);
	if(str.at(0) == "-")
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


	if(!channel.hasUser(user))
		return (ERR_NOT_MEMBER_OF_CHANNEL(user, channelName));
	if(!channel.isUserOp(user))
		return (ERR_USER_NOT_OPERATOR(user));

	std::string modeStr = message.getParamAt(1);

	if(modeStr.length() != 2)
		// TODO: better response?
		return (ERR_UNKNOWN_COMMAND(user, "MODE"));

	Mode mode = getModeFromString(modeStr);
	ModeType modeType = getModeTypeFromString(modeStr);

	if(mode == MODE_INVALID || modeType == MODE_TYPE_INVALID)
		// TODO: better response?
		return (ERR_UNKNOWN_COMMAND(user, "MODE"));

	Channel &channel = channelManager.getChannelWithName(channelName);

	if(mode == MODE_INVITE)
	{
		channel.setInviteOnly(modeType == MODE_TYPE_SET);
		return (MODE_SET_INVITE_ONLY(user, channel, modeType == MODE_TYPE_SET));
	}
	if(mode == MODE_TOPIC)
	{
		channel.setTopicRestricted(modeType == MODE_TYPE_SET);
		return (MODE_SET_TOPIC_RESTRICTED(user, channel, modeType == MODE_TYPE_SET));
	}
	if(mode == MODE_KEY)
	{
		if(modeType == MODE_TYPE_SET && message.getParamCount() < 3)
			return (ERR_NEED_MORE_PARAMS(user, "MODE"));

		channel.setKeyRequired(modeType == MODE_TYPE_SET);

		if(modeType == MODE_TYPE_SET)
			channel.setKey(message.getParamAt(2));
		else
			channel.setKey("");
		return (MODE_SET_KEY_REQUIRED(user, channel, modeType == MODE_TYPE_SET));
	}
	if(mode == MODE_OP)
	{
		if(message.getParamCount() < 3)
			return (ERR_NEED_MORE_PARAMS(user, "MODE"));

		UserManager &userManager = UserManager::getInstance();
		const std::string &targetName = message.getParamAt(2);

		if(!userManager.hasUserWithNickname(targetName))
			return (ERR_NICK_DOESNT_EXIST(user, targetName));

		User &target = userManager.getUserByNickname(targetName);

		if(!channel.hasUser(target))
			return (ERR_NOT_MEMBER_OF_CHANNEL(user, channelName));

		if(modeType == MODE_TYPE_SET)
		{
			if(channel.isUserOp(target))
				return (ERR_USER_ALREADY_OPERATOR(user, channel, targetName));
			channel.addOperator(target);
		}
		else
		{
			if(channel.isUserOp(target))
				return (ERR_USER_NOT_OPERATOR(user));
			channel.removeOperator(target);
		}
		return (MODE_SET_OP(user, channel, target, modeType == MODE_TYPE_SET));
	}
	if(mode == MODE_LIMIT)
	{
		if(modeType == MODE_TYPE_SET && message.getParamCount() < 3)
			return (ERR_NEED_MORE_PARAMS(user, "MODE"));

		channel.setUserLimit(modeType == MODE_TYPE_SET);

		if(modeType == MODE_TYPE_SET)
			channel.setLimit(std::stoi(message.getParamAt(2)));
		else
			channel.setLimit(0);
		return (MODE_SET_USER_LIMIT(user, channel, modeType == MODE_TYPE_SET));
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

// std::string addSpaceAfterColon(std::string str) {
//     size_t pos = str.find(':');
//     if (pos != std::string::npos && pos < str.size() - 1 && str[pos + 1] != ' ') {
//         str.insert(pos + 1, " ");
//     }
//     return str;
// }

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
        user.queue(ERR_UNKNOWN_COMMAND(user, message.getCommand())); //unknown command
}

#endif
