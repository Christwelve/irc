#include "Channel.hpp"
#include "UserManager.hpp"
#include "MessageDefines.hpp"
#include <algorithm>

Channel::Channel(const std::string &name)
{
	this->name_ = name;
	this->key_ = "";
	this->topic_ = "";
	this->limit_ = CHANNEL_DEFAULT_LIMIT;
	this->i_ = false;
	this->t_ = false;
	this->k_ = false;
	this->l_ = false;
}

Channel::Channel(const std::string &name, const std::string &key)
{
	this->name_ = name;
	this->key_ = key;
	this->topic_ = "";
	this->limit_ = CHANNEL_DEFAULT_LIMIT;
	this->i_ = false;
	this->t_ = false;
	this->k_ = true;
	this->l_ = false;
}

Channel::~Channel() { return; }


void Channel::addUser(const User &user)
{
	users_.push_back(user);
}

void Channel::addOperator(const User &user)
{
	operators_.push_back(user);
}

void Channel::addInvite(const User &user)
{
	invites_.push_back(user);
}

void Channel::removeUser(const User &user)
{
	removeOperator(user);
	removeInvite(user);

	for (unsigned long i = 0; i < users_.size(); i++)
	{
		if (users_.at(i) == user)
		{
			users_.erase(users_.begin() + i);
			return;
		}
	}
}

void Channel::removeOperator(const User &user)
{
	for (unsigned long i = 0; i < operators_.size(); i++)
	{
		if (operators_.at(i) == user)
		{
			operators_.erase(operators_.begin() + i);
			return;
		}

	}
}

void Channel::removeInvite(const User &user)
{
	for (unsigned long i = 0; i < invites_.size(); i++)
	{
		if (invites_.at(i) == user)
		{
			invites_.erase(invites_.begin() + i);
			return;
		}

	}
}


std::vector<User> &Channel::getUsers(void) { return (users_); }

std::vector<User> &Channel::getOperators(void) { return (operators_); }

std::vector<User> &Channel::getInvites(void) { return (invites_); }


void Channel::setKey(const std::string &key) { key_ = key; }

void Channel::setTopic(const std::string &topic) { topic_ = topic; }

void Channel::setLimit(unsigned int limit) { limit_ = limit; }

void Channel::setInviteOnly(bool flag) { i_ = flag; }

void Channel::setTopicRestricted(bool flag) { t_ = flag; }

void Channel::setKeyRequired(bool flag) { k_ = flag; }

void Channel::setUserLimit(bool flag) { l_ = flag; }


const std::string &Channel::getName(void) const { return (name_); }

const std::string &Channel::getKey(void) const { return (key_); }

const std::string &Channel::getTopic(void) const { return (topic_); }

bool Channel::isInviteOnly(void) const { return (i_); }

bool Channel::isTopicRestricted(void) const { return (t_); }

bool Channel::isKeyRequired(void) const { return (k_); }

bool Channel::isUserLimit(void) const { return (l_); }


void Channel::broadcastMessage(const User &user, const std::string &message)
{
	for (unsigned long i = 0; i < users_.size(); i++)
	{
		User &target = UserManager::getInstance().getUserByNickname(users_.at(i).getNickname());

		if (target != user)
			target.queue(PRIVMSG_SEND_MESSAGE(user, name_, message));
	}
}

void Channel::sendMessage(const std::string &message)
{
	for (unsigned long i = 0; i < users_.size(); i++)
	{
		User &target = UserManager::getInstance().getUserByNickname(users_.at(i).getNickname());

		target.queue(message);
	}
}

bool Channel::isFull(void) const { return (users_.size() >= limit_); }

bool Channel::hasUser(const User &user) const
{
	return (std::find(users_.begin(), users_.end(), user) != users_.end());
}

bool Channel::isEmpty(void) const { return (users_.size() == 0); }

bool Channel::isKeyValid(const std::string &key) const
{
	return (key == key_);
}

bool Channel::isUserOp(const User &user) const
{
	return (std::find(operators_.begin(), operators_.end(), user) != operators_.end());
}

bool Channel::isUserInvited(const User &user) const
{
	return (std::find(invites_.begin(), invites_.end(), user) != invites_.end());
}

std::string Channel::getUserList(void) const
{
	std::string list;

	for (unsigned long i = 0; i < users_.size(); i++)
	{
		User user = users_.at(i);

		list += i == 0 ? "" : " ";
		list += isUserOp(user) ? "@" : "";
		list += user.getNickname();
	}

	return (list);
}

bool Channel::hasOperator(void) const
{
	return (operators_.size() > 0);
}
