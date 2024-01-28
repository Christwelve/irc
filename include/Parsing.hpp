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


class Server;

std::string pass(User &user, const Message &message)
{
	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "PASS"));
	if(user.isRegistered())
		return (ERR_USER_ALREADY_REGISTERED(user));
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

	user.setNickname(message.getParamAt(0));
	user.setState(USER_EXPECTS_USER);

	return ("");
}

std::string userCmd(User &user, const Message &message)
{
	if (message.getParamCount() < 3)
		return (ERR_NEED_MORE_PARAMS(user, "USER"));
	if (user.isRegistered())
		return (ERR_USER_ALREADY_REGISTERED(user));
	if (!user.hasState(USER_EXPECTS_USER))
		return (ERR_UNKNOWN_COMMAND(user, "USER"));

	user.setUsername(message.getParamAt(0), message.getParamAt(2));
	user.setState(USER_REGISTERED);
	// TODO: message
	user.queue(":" + std::string(SERVER_NAME) + " 001 " + user.getNickname() + " :Ey Mate! Welcome to 👁️  ®️  🌊");

	return ("");
}

std::string join(User &user, const Message &message)
{
	(void)user;
	std::cout << "join() " << message.getParamAt(0) << std::endl;

	return ("");
}

std::string part(User &user, const Message &message)
{
	(void)user;

	std::cout << "part() " << message.getParamAt(0) << std::endl;

	return ("");
}

std::string privmsg(User &user, const Message &message)
{
	(void)user;
	std::cout << "privmsg() " << message.getParamAt(0) << message.getParamAt(1) << std::endl;

	return ("");
}

std::string kick(User &user, const Message &message)
{
	(void)user;
	std::cout << "kick() " << message.getParamAt(0) << message.getParamAt(1) << message.getParamAt(2) << std::endl;

	return ("");
}

std::string invite(User &user, const Message &message)
{
	(void)user;
	std::cout << "invite() " << message.getParamAt(0) << message.getParamAt(1) << std::endl;

	return ("");
}

std::string topic(User &user, const Message &message)
{
	(void)user;

	std::cout << "topic() " << message.getParamAt(0) << message.getParamAt(1) << std::endl;

	return ("");
}
std::string list(User &user, const Message &message)
{
	(void)user;
	(void)message;
	std::cout << "list() " << std::endl;

	return ("");
}

std::string mode(User &user, const Message &message)
{
	(void)user;

	std::cout << "mode() " << message.getParamAt(0) << message.getParamAt(1) << std::endl;

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

	if (user.hasState(USER_EXPECTS_PASS) && message.getCommand() != "PASS")
    {
        // TODO: send error message
        user.queue(ERR_UNKNOWN_COMMAND(user, message.getCommand()));
        UserManager::getInstance().removeUser(user);
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
