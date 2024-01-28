#ifndef USERMANAGER_HPP
# define USERMANAGER_HPP

# include <vector>
# include <map>
# include <string>
# include <poll.h>
# include "User.hpp"
# include "Socket.hpp"

class UserManager
{
	private:
		UserManager(void);

		std::vector<User> users_;

	public:
		static UserManager &getInstance(void);

		~UserManager();

		void createUserFromSocket(const Socket &socket);
		bool hasUserWithNickname(const std::string &nickname) const;
		User &getUserByNickname(const std::string &nickname);
		void deleteUserBySocket(const Socket &socket);

		void removeUser(User &user);

		struct pollfd *getPollFdsWithServerSocket(const Socket &serverSocket);
		void setPollFdsWithServerSocket(Socket &serverSocket, struct pollfd *pollfds);

		std::vector<User> &getUsers(void);
		size_t getUserCount(void) const;
};

#endif
