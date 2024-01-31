#include "Command.hpp"
#include "ChannelManager.hpp"
#include "MessageDefines.hpp"

std::string Command::quit(User &user, const Message &message)
{
	send(user.getSocket().getFd(), ERR_QUIT_SERVER(user).c_str(), ERR_QUIT_SERVER(user).length(), 0);
	ChannelManager::getInstance().removeUserFromAllChannels(user, message.getTrailing());
	user.remove();

	return ("");
}
