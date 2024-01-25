#include "UserManager.hpp"

UserManager::UserManager(void) {}

UserManager::~UserManager() {}

UserManager &UserManager::getInstance(void)
{
	static UserManager instance;
	return instance;
}

void UserManager::createUserFromSocket(const Socket &socket)
{
	users_.push_back(User(socket));
}

const User *UserManager::getUserByNickname(const std::string &nickname) const
{
	for (unsigned long i = 0; i < users_.size(); i++)
	{
		if (users_.at(i)->getNickname() == nickname)
			return (users_.at(i));
	}

	return (NULL);
}

void UserManager::deleteUserBySocket(const Socket &socket)
{
	for (unsigned long i = 0; i < users_.size(); i++)
	{
		if (users_.at(i)->getSocket() != socket)
			continue;

		users_.erase(users_.begin() + i);
	}
}
