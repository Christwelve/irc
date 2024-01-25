#ifndef USER_HPP
# define USER_HPP

# include <string>
# include "Socket.hpp"

class User
{
	private:

		Socket *socket_;

		bool is_approved_;
		bool is_registered_;

		std::string nickname_;
		std::string username_;
		std::string realname_;

		std::string command_buffer_;

	public:
		User(Socket *socket);
		~User();

		Socket *getSocket(void) const;
		void setSocket(Socket *socket);

		bool isApproved(void) const;
		bool isRegistered(void) const;
		bool isOnline(void) const;
		void setApproved(void);
		void setRegistered(void);

		const std::string &getNickname(void) const;
		const std::string &getUsername(void) const;
		const std::string &getRealname(void) const;
		void setNickname(const std::string &nickname);
		void setUsername(const std::string &username);
		void setRealname(const std::string &realname);

		void appendCommandBuffer(const std::string &partial_command);

};

#endif
