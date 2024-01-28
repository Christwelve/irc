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
		static bool isValidChannelName(const std::string &name);

		~ChannelManager();

		bool hasChannelWithName(const std::string &name) const;
		Channel &createChannelWithName(const std::string &name);
		Channel &getChannelByName(const std::string &name);
		void removeChannel(const Channel &channel);

		void addUserToChannel(Channel &channel, const User &user);
		void removeUserFromChannel(Channel &channel, const User &user);
		void removeUserFromAllChannels(const User &user);
		void sendMessageToChannel(Channel &channel, const User &user, const std::string &message);
};

#endif
