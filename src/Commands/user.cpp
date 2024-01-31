#include "Command.hpp"
#include "MessageDefines.hpp"

std::string Command::user(User &user, const Message &message)
{
	if (user.isRegistered())
		return (ERR_USER_ALREADY_REGISTERED(user));
	if (message.getParamCount() < 3)
		return (ERR_NEED_MORE_PARAMS(user, "USER"));
	if (!message.hasTrailing())
		return (ERR_TRAILING_MISSING(user, "USER"));
	if (!user.hasState(USER_EXPECTS_USER))
		return (ERR_UNKNOWN_COMMAND(user, "USER"));

	user.setUsername(message.getParamAt(0), message.getParamAt(2));
	user.setState(USER_REGISTERED);

	user.queue(USER_WELCOME(user));
	user.queue(USER_WELCOME_002(user));
	user.queue(USER_WELCOME_005(user));

	return ("");
}
