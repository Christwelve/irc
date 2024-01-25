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

		// std::map<const std::string, void ()>

	public:
		static UserManager &getInstance(void);

		~UserManager();

		void createUserFromSocket(const Socket &socket);
		std::vector<User>::const_iterator getUserByNickname(const std::string &nickname) const;
		void deleteUserBySocket(const Socket &socket);

		std::vector<User> &getUsers(void);
		void removeUser(User &user);

		struct pollfd *getPollFdsWithServerSocket(const Socket &serverSocket);
		void setPollFdsWithServerSocket(Socket &serverSocket, struct pollfd *pollfds);

		size_t getUserCount(void) const;
};

#endif
