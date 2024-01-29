#include "UserManager.hpp"
#include "IRCError.hpp"

UserManager::UserManager(void) {}

UserManager::~UserManager() {}

UserManager &UserManager::getInstance(void)
{
	static UserManager instance;
	return instance;
}

bool UserManager::isValidNickname(const std::string &name)
{
	static const std::string validNicknameCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-";

	if (name.length() < 1 || name.length() > 50)
		return false;

	for (unsigned long i = 0; i < name.length(); i++)
	{
		if(validNicknameCharacters.find(name[i]) == std::string::npos)
			return false;
	}

	return true;
}

void UserManager::createUserFromSocket(const Socket &socket)
{
	users_.push_back(User(socket));
}

bool UserManager::hasUserWithNickname(const std::string &nickname) const
{
	for(std::vector<User>::const_iterator it = users_.begin(); it != users_.end(); it++)
	{
		if (it->getNickname() == nickname)
			return true;
	}

	return false;
}

User &UserManager::getUserByNickname(const std::string &nickname)
{
	for(std::vector<User>::iterator it = users_.begin(); it != users_.end(); it++)
	{
		if (it->getNickname() == nickname)
			return *it;
	}

	throw IRCError("UserManager::getUserByNickname: no user with nickname " + nickname);
}

void UserManager::deleteUserBySocket(const Socket &socket)
{
	for(std::vector<User>::iterator it = users_.begin(); it != users_.end(); it++)
	{
		Socket userSocket = it->getSocket();

		if (userSocket != socket)
			continue;

		userSocket.close();
		users_.erase(it);
		break;
	}
}

struct pollfd *UserManager::getPollFdsWithServerSocket(const Socket &serverSocket)
{
	struct pollfd *pollfds = new struct pollfd[users_.size() + 1];

	pollfds[0] = serverSocket.getPollFd();

	for (unsigned long i = 0; i < users_.size(); i++)
		pollfds[i + 1] = users_.at(i).getSocket().getPollFd();

	return (pollfds);
}

void UserManager::setPollFdsWithServerSocket(Socket &serverSocket, struct pollfd *pollfds)
{
	serverSocket.setState(pollfds[0].revents);

	for (unsigned long i = 0; i < users_.size(); i++)
		users_.at(i).getSocket().setState(pollfds[i + 1].revents);
}

std::vector<User> &UserManager::getUsers(void)
{
	return (users_);
}

void UserManager::removeUser(User &user)
{
	for(std::vector<User>::iterator it = users_.begin(); it != users_.end(); it++)
	{
		if (*it != user)
			continue;

		user.getSocket().close();
		users_.erase(it);

		break;
	}
}

size_t UserManager::getUserCount(void) const
{
	return (users_.size());
}
