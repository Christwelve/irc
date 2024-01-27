#include "ChannelManager.hpp"

ChannelManager::ChannelManager(void) {}

ChannelManager::~ChannelManager() {}

ChannelManager &ChannelManager::getInstance(void)
{
	static ChannelManager instance;
	return instance;
}

void ChannelManager::createChannel(const std::string &name)
{
	channels_.insert(std::pair<const std::string, Channel>(name, Channel(name)));
}

const Channel &ChannelManager::getChannelByName(const std::string &name) const
{
	return channels_.at(name);
}

void ChannelManager::removeChannel(const Channel &channel)
{
	// TODO: remove all users from channel
	channels_.erase(channel.getName());
}

void ChannelManager::addUserToChannel(const std::string &channelName, const User &user)
{
	channels_.at(channelName).addUser(user);
}

void ChannelManager::removeUserFromChannel(const std::string &channelName, const User &user)
{
	// TODO: notify users that user has left
	channels_.at(channelName).removeUser(user);
}

void ChannelManager::sendMessageToChannel(const std::string &channelName, const User &user, const std::string &message)
{
	channels_.at(channelName).sendMessage(user, message);
}
