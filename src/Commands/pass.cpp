#include "Command.hpp"
#include "MessageDefines.hpp"
#include "UserManager.hpp"


std::string Command::pass(User &user, const Message &message)
{
	if(user.isRegistered())
		return (ERR_USER_ALREADY_REGISTERED(user));
	if(message.getParamCount() < 1)
		return (ERR_NEED_MORE_PARAMS(user, "PASS"));
	if(!user.hasState(USER_EXPECTS_PASS))
		return (ERR_UNKNOWN_COMMAND(user, "PASS"));
	if(!Server::getInstance().isPasswordValid(message.getParamAt(0)))
	{
		send(user.getSocket().getFd(), ERR_PASS_INVALID(user).c_str(), ERR_PASS_INVALID(user).length(), 0);
		user.remove();
		return ("");
	}

	user.setState(USER_EXPECTS_NICK);

	return ("");
}
