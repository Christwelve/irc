#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <vector>
# include <string>

# include "User.hpp"

# define CHANNEL_DEFAULT_LIMIT 50

class Channel
{
	private:

		std::vector<User> users_;
		std::vector<User> operators_;
		std::vector<User> invites_;

		std::string name_;
		std::string key_;
		std::string topic_;
		unsigned int limit_;

		bool i_;
		bool t_;
		bool k_;
		bool l_;

	public:

		Channel(const std::string &name);
		Channel(const std::string &name, const std::string &key);
		~Channel();

		void addUser(const User &user);
		void addOperator(const User &user);
		void addInvite(const User &user);

		void removeUser(const User &user);
		void removeOperator(const User &user);
		void removeInvite(const User &user);

		std::vector<User> &getUsers(void);
		std::vector<User> &getOperators(void);
		std::vector<User> &getInvites(void);

		void setKey(const std::string &key);
		void setTopic(const std::string &topic);
		void setLimit(unsigned int limit);

		void setInviteOnly(bool flag);
		void setTopicRestricted(bool flag);
		void setKeyRequired(bool flag);
		void setUserLimit(bool flag);

		const std::string &getName(void) const;
		const std::string &getKey(void) const;
		const std::string &getTopic(void) const;

		bool isInviteOnly(void) const;
		bool isTopicRestricted(void) const;
		bool isKeyRequired(void) const;
		bool isUserLimit(void) const;

		void broadcastMessage(const User &user, const std::string &message);
		void sendMessage(const std::string &message);
		void sendCommand(const std::string &message);

		bool isFull(void) const;
		bool hasUser(const User &user) const;
		bool isEmpty(void) const;
		bool isKeyValid(const std::string &key) const;
		bool isUserOp(const User &user) const;
		bool isUserInvited(const User &user) const;
		std::string getUserList(void) const;
};


#endif
