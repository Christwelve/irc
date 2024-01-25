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

		void addUser(User &user);
		void addOperator(User &user);
		void addInvite(User &user);

		void removeUser(User &user);
		void removeOperator(User &user);
		void removeInvite(User &user);

		const std::vector<User> &getUsers(void) const;
		const std::vector<User> &getOperators(void) const;
		const std::vector<User> &getInvites(void) const;

		void setKey(const std::string &key);
		void setTopic(const std::string &topic);
		void setLimit(unsigned int limit);

		void setInviteOnly(void);
		void setTopicRestricted(void);
		void setKeyRequired(void);
		void setUserLimit(void);

		const std::string &getName(void) const;
		const std::string &getKey(void) const;
		const std::string &getTopic(void) const;
		unsigned int getLimit(void) const;

		bool isInviteOnly(void) const;
		bool isTopicRestricted(void) const;
		bool isKeyRequired(void) const;
		bool isUserLimit(void) const;
		
};


#endif
