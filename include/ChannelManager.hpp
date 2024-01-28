#ifndef CHANNELMANAGER_HPP
# define CHANNELMANAGER_HPP

# include <map>
# include <string>
# include "Channel.hpp"

class ChannelManager
{
	private:
		ChannelManager(void);

		std::map<const std::string, Channel> channels_;

	public:
		static ChannelManager &getInstance(void);

		~ChannelManager();

		bool hasChannelWithName(const std::string &name) const;
		void createChannel(const std::string &name);
		const Channel &getChannelByName(const std::string &name) const;
		void removeChannel(const Channel &channel);

		void addUserToChannel(const std::string &channelName, const User &user);
		void removeUserFromChannel(const std::string &channelName, const User &user);
		void sendMessageToChannel(const std::string &channelName, const User &user, const std::string &message);
};

#endif
