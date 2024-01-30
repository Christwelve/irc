#include "Command.hpp"
#include "ChannelManager.hpp"

std::string Command::quit(User &user, const Message &message)
{
	ChannelManager::getInstance().removeUserFromAllChannels(user, message.getTrailing());

	return ("");
}
