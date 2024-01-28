#ifndef USER_HPP
# define USER_HPP

# include <string>
# include <queue>
# include "Socket.hpp"

enum UserState
{
	USER_EXPECTS_PASS,
	USER_EXPECTS_NICK,
	USER_EXPECTS_USER,
	USER_REGISTERED
};

class User
{
	private:

		Socket socket_;

		UserState state_;

		std::string nickname_;
		std::string username_;
		std::string realname_;

		std::string commandBuffer_;
		std::queue<std::string> messageQueue_;

	public:
		User(const Socket &socket);
		~User();

		Socket &getSocket(void);

		bool isRegistered(void) const;
		void setState(UserState state);
		bool hasState(UserState state) const;

		const std::string &getNickname(void) const;
		const std::string &getUsername(void) const;
		const std::string &getRealname(void) const;
		std::string getUserIdent(void) const;
		void setNickname(const std::string &nickname);
		void setUsername(const std::string &username, const std::string &realname);

		void appendCommandBuffer(const std::string &partialCommand);
		void queue(const std::string &message);
		std::string &getNextMessage(void);
		void finishedSendingMessage(void);
		bool hasInput(void) const;
		bool hasOutput(void) const;
		std::string getInputFromCommandBuffer(void);

		void remove(void);

		bool operator==(const User &user) const;
		bool operator!=(const User &user) const;
};

#endif
