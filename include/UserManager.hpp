#ifndef USERMANAGER_HPP
# define USERMANAGER_HPP

# include <vector>
# include <map>
# include <string>
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
		const User &getUserByNickname(const std::string &nickname) const;
		void deleteUserBySocket(const Socket &socket);
};

#endif
