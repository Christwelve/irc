#include "Command.hpp"
#include "MessageDefines.hpp"

std::string Command::ping(User &user, const Message &message)
{
	if (message.getParamCount() == 1)
		return (PONG_WITH_PARAM(message));
	if (message.hasTrailing())
		return (PONG_WITH_TRAILING(message));

	return (ERR_NEED_MORE_PARAMS(user, "PING"));
}
