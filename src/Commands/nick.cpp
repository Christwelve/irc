#include "Command.hpp"
#include "UserManager.hpp"
#include "MessageDefines.hpp"

std::string Command::nick(User &user, const Message &message)
{
	UserManager &userManager = UserManager::getInstance();

	if (message.getParamCount() == 0)
		return (ERR_NEED_MORE_PARAMS(user, "NICK"));

	const std::string &nickname = message.getParamAt(0);

	if(!UserManager::isValidNickname(nickname))
		return (ERR_INVALID_PARAM(user, nickname));
	if (userManager.hasUserWithNickname(nickname))
		return (ERR_NICK_ALREADY_IN_USE(user, nickname));
	if (user.isRegistered())
	{
		user.queue(NICK_CHANGED(user, nickname));

		std::vector<User> &users = UserManager::getInstance().getUsers();

		for(unsigned long i = 0; i < users.size(); i++)
		{
			User &target = users.at(i);

			target.queue(NICK_CHANGED(user, nickname));
		}

		user.setNickname(nickname);

		return ("");
	}
	if (!user.hasState(USER_EXPECTS_NICK))
		return (ERR_UNKNOWN_COMMAND(user, "NICK"));

	user.setNickname(nickname);
	user.setState(USER_EXPECTS_USER);

	return ("");
}
