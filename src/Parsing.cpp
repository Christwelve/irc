#include "Command.hpp"
#include "MessageDefines.hpp"


void Command::parseInput(User &user, std::string input)
{
	static std::map<std::string, CommandFunc> commandMap = getCommandMap();

	Message message(input);

	if(user.hasState(USER_EXPECTS_PASS) && message.getCommand() != "PASS")
	{
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


std::map <std::string, CommandFunc> Command::getCommandMap()
{
	std::map<std::string, CommandFunc> map;

	map["PASS"] = &Command::pass;
	map["PING"] = &Command::ping;
	map["CAP" ] = &Command::cap;
	map["QUIT"] = &Command::quit;
	map["NICK"] = &Command::nick;
	map["USER"] = &Command::user;
	map["JOIN"] = &Command::join;
	map["PART"] = &Command::part;
	map["PRIVMSG"] = &Command::privmsg;
	map["KICK"] = &Command::kick;
	map["INVITE"] = &Command::invite;
	map["TOPIC"] = &Command::topic;
	map["MODE"] = &Command::mode;

	return (map);
}
