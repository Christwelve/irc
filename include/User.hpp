#ifndef USER_HPP
# define USER_HPP

# include <string>

class User
{
	private:

		int socket_fd_;

		bool is_approved_;
		bool is_registered_;

		std::string nickname_;
		std::string username_;
		std::string realname_;

		std::string command_buffer_;

	public:
		User(int socket_fd);
		User(const User &user);
		User &operator=(const User &user);
		~User();

		int getSocketFd(void) const;

		bool isApproved(void) const;
		bool isRegistered(void) const;
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
