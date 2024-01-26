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

std::vector<User>::iterator UserManager::getUserByNickname(const std::string &nickname)
{
	for(std::vector<User>::iterator it = users_.begin(); it != users_.end(); it++)
	{
		if (it->getNickname() == nickname)
			return (it);
	}

	return (users_.end());
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
