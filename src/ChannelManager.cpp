#include "ChannelManager.hpp"
#include "MessageDefines.hpp"

ChannelManager::ChannelManager(void) {}

ChannelManager::~ChannelManager() {}

ChannelManager &ChannelManager::getInstance(void)
{
	static ChannelManager instance;
	return instance;
}

bool ChannelManager::isValidChannelName(const std::string &name)
{
	static const std::string validChannelNameCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-";

	if (name.length() < 2 || name.length() > 50)
		return false;

	if (name[0] != '#')
		return false;

	for (unsigned long i = 1; i < name.length(); i++)
	{
		if(validChannelNameCharacters.find(name[i]) == std::string::npos)
			return false;
	}

	return true;
}

bool ChannelManager::hasChannelWithName(const std::string &name) const
{
	return channels_.find(name) != channels_.end();
}

Channel &ChannelManager::createChannelWithName(const std::string &name)
{
	channels_.insert(std::pair<const std::string, Channel>(name, Channel(name)));

	return getChannelByName(name);
}

Channel &ChannelManager::getChannelByName(const std::string &name)
{
	return channels_.at(name);
}

void ChannelManager::removeChannel(const Channel &channel)
{
	// TODO: remove all users from channel
	channels_.erase(channel.getName());
}

void ChannelManager::addUserToChannel(Channel &channel, const User &user)
{
	channel.addUser(user);
}

void ChannelManager::removeUserFromChannel(Channel &channel, const User &user)
{
	channel.removeUser(user);
}

void ChannelManager::removeUserFromAllChannels(const User &user, const std::string &quitMessage)
{
	for (std::map<const std::string, Channel>::iterator it = channels_.begin(); it != channels_.end(); ++it)
	{
		Channel &channel = it->second;

		if(!channel.hasUser(user))
			continue;

		channel.removeUser(user);
		channel.sendMessage(QUIT(user, quitMessage));
	}
}
