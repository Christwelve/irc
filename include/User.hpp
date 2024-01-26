#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <queue>
# include "Socket.hpp"

class User
{
	private:

		Socket socket_;

		bool isApproved_;
		bool isRegistered_;

		std::string nickname_;
		std::string username_;
		std::string realname_;

		std::string commandBuffer_;
		std::queue<std::string> messageQueue_;

	public:
		User(const Socket &socket);
		~User();

		Socket &getSocket(void);

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

		void appendCommandBuffer(const std::string &partialCommand);
		void queue(const std::string &message);
		std::string &getNextMessage(void);
		void finishedSendingMessage(void);
		bool hasInput(void) const;
		bool hasOutput(void) const;
		std::string getInputFromCommandBuffer(void);

		bool operator==(const User &user) const;
		bool operator!=(const User &user) const;
};

#endif
