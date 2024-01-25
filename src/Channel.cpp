#include "../include/Channel.hpp"

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



// CONTAINER MODIFIERS

void Channel::addUser(User &user)
{
	users_.push_back(user);
}

void Channel::addOperator(User &user)
{
	operators_.push_back(user);
}

void Channel::addInvite(User &user)
{
	invites_.push_back(user);
}

void Channel::removeUser(User &user)
{
	for (unsigned long i = 0; i < users_.size(); i++)
	{
		if (users_.at(i).getNickname() == user.getNickname())
		{
			users_.erase(users_.begin() + i);
			return;
		}

	}
}

void Channel::removeOperator(User &user)
{
	for (unsigned long i = 0; i < operators_.size(); i++)
	{
		if (operators_.at(i).getNickname() == user.getNickname())
		{
			operators_.erase(operators_.begin() + i);
			return;
		}

	}
}

void Channel::removeInvite(User &user)
{
	for (unsigned long i = 0; i < invites_.size(); i++)
	{
		if (invites_.at(i).getNickname() == user.getNickname())
		{
			invites_.erase(invites_.begin() + i);
			return;
		}

	}
}

// CONTAINER GETTERS

const std::vector<User> &Channel::getUsers(void) const { return (users_); }

const std::vector<User> &Channel::getOperators(void) const { return (operators_); }

const std::vector<User> &Channel::getInvites(void) const { return (invites_); }

// CHANNEL MODIFIERS

void Channel::setKey(const std::string &key) { key_ = key; }

void Channel::setTopic(const std::string &topic) { topic_ = topic; }

void Channel::setLimit(unsigned int limit) { limit_ = limit; }

void Channel::setInviteOnly(void) { i_ = true; }

void Channel::setTopicRestricted(void) { t_ = true; }

void Channel::setKeyRequired(void) { k_ = true; }

void Channel::setUserLimit(void) { l_ = true; }

// CHANNEL GETTERS

const std::string &Channel::getName(void) const { return (name_); }

const std::string &Channel::getKey(void) const { return (key_); }

const std::string &Channel::getTopic(void) const { return (topic_); }

unsigned int Channel::getLimit(void) const { return (limit_); }

bool Channel::isInviteOnly(void) const { return (i_); }

bool Channel::isTopicRestricted(void) const { return (t_); }

bool Channel::isKeyRequired(void) const { return (k_); }

bool Channel::isUserLimit(void) const { return (l_); }