#include "ChannelManager.hpp"

ChannelManager::ChannelManager(void) {}

ChannelManager::~ChannelManager() {}

ChannelManager &ChannelManager::getInstance(void)
{
	static ChannelManager instance;
	return instance;
}

bool ChannelManager::hasChannelWithName(const std::string &name) const
{
	return (channels_.find(name) != channels_.end());
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

	// TODO: add proper channel leave message
	channel.sendMessage(user, user.getNickname() + " has joined the channel");
}

void ChannelManager::removeUserFromChannel(const std::string &channelName, const User &user, const std::string &message)
{
	Channel &channel = channels_.at(channelName);
	channel.removeUser(user);

	// TODO: add proper channel leave message
	channel.sendMessage(user, user.getNickname() + " has left the channel - " + message);
}

void ChannelManager::sendMessageToChannel(const std::string &channelName, const User &user, const std::string &message)
{
	channels_.at(channelName).sendMessage(user, message);
}

