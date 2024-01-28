#ifndef MESSAGEDEFINES_HPP
# define MESSAGEDEFINES_HPP

# include <string>
# include "Server.hpp"

# define PONG_WITH_TRAILING(message) ("PONG " + std::string(SERVER_NAME) + " :" + message.getTrailing())
# define PONG_WITH_PARAM(message) ("PONG " + std::string(SERVER_NAME) + " " + message.getParamAt(0))
# define NICK_CHANGED(user, nickname) (":" + user.getUserIdent() + " NICK :" + nickname)
# define USER_CHANGED(user, username, hostname, servername, realname) (":" + user.getUserIdent() + " USER " + username + " " + hostname + " " + servername + " :" + realname)
# define JOIN_CHANNEL(user, channel) (":" + user.getUserIdent() + " JOIN " + channel)
# define PART_CHANNEL(user, channel, comment) (":" + user.getUserIdent() + " PART " + channel + " :" + comment)
# define PRIVMSG_SEND_MESSAGE(user, target, message) (":" + user.getUserIdent() + " PRIVMSG " + target + " :" + message)
# define KICK_USER(user, channel, target, comment) (":" + user.getUserIdent() + " KICK " + channel + " " + target + " :" + comment)
# define INVITE_USER(user, target, channel) (":" + user.getUserIdent() + " INVITE " + target + " " + channel)
# define TOPIC_CHANGED(user, channel, topic) (":" + user.getUserIdent() + " TOPIC " + channel + " :" + topic)
# define LIST_CHANNELS(user, channel) (":" + user.getUserIdent() + " LIST " + channel)
# define MODE_CHANNEL(user, channel, modes) (":" + user.getUserIdent() + " MODE " + channel + " " + modes)

# define ERR_NICK_ALREADY_IN_USE(user, nickname) (":" + std::string(SERVER_NAME) + " 433 " + user.getNickname() + " " + nickname + " :Nickname is already in use")
# define ERR_NICK_DOESNT_EXIST(user, nickname) (":" + std::string(SERVER_NAME) + " 401 " + user.getNickname() + " " + nickname + " :This nickname doesn't exist")
# define ERR_USER_ALREADY_REGISTERED(user) (":" + std::string(SERVER_NAME) + " 462 " + user.getNickname() + " :Already registered")
# define ERR_CHANNEL_INVALID_NAME(user, channel) (":" + std::string(SERVER_NAME) + " 480 " + user.getNickname() + " " + channel + " :Invalid Channel Name")
# define ERR_CHANNEL_INVALID_KEY(user, channel) (":" + std::string(SERVER_NAME) + " 479 " + user.getNickname() + " " + channel + " :Invalid Channel Key")
# define ERR_NOT_MEMBER_OF_CHANNEL(user, channel) (":" + std::string(SERVER_NAME) + " 442 " + user.getNickname() + " " + channel + " :You're not a member of this channel")
# define ERR_CHANNEL_DOESNT_EXIST(user, channel) (":" + std::string(SERVER_NAME) + " 403 " + user.getNickname() + " " + channel + " :Channel doesn't exist")
# define ERR_CHANNEL_NOT_OPERATOR(user, channel) (":" + std::string(SERVER_NAME) + " 482 " + user.getNickname() + " " + channel + " :You're not channel operator")
# define ERR_USER_NOT_IN_CHANNEL(user, target, channel) (":" + std::string(SERVER_NAME) + " 441 " + user.getNickname() + " " + target + " " + channel + " :User isn't on that channel")
# define ERR_USER_IS_ALREADY_ON_CHANNEL(user, target, channel) (":" + std::string(SERVER_NAME) + " 443 " + user.getNickname() + " " + target + " " + channel + " :User is already on that channel")
# define ERR_CHANNEL_IS_FULL(user, channel) (":" + std::string(SERVER_NAME) + " 471 " + user.getNickname() + " " + channel + " :Channel is full")


# define ERR_PRIVMSG_NO_TEXT_TO_SEND(user) (":" + std::string(SERVER_NAME) + " 412 " + user.getNickname() + " :No text to send")
# define ERR_NOT_ON_CHANNEL(user, channel) (":" + std::string(SERVER_NAME) + " 442 " + user.getNickname() + " " + channel + " :You're not on that channel")

# define ERR_NEED_MORE_PARAMS(user, command) (":" + std::string(SERVER_NAME) + " 461 " + user.getNickname() + " " + command + " :Not enough parameters")
# define ERR_UNKNOWN_COMMAND(user, command) (":" + std::string(SERVER_NAME) + " 421 " + user.getNickname() + " " + command + " :Unknown command")

// TODO: Check if necessary:

// ERR_NONICKNAMEGIVEN (431)
// ERR_ERRONEUSNICKNAME (432)
// ERR_ALREADYREGISTRED (462)
// ERR_UNKNOWNMODE (472)
// ERR_INVITEONLYCHAN (473)
// ERR_BADCHANNELKEY (475)
// ERR_BADCHANMASK (476)
// ERR_ERRONEUSCHANNELNAME (480)
// ERR_ERRONEUSCHANNELKEY (479)
// ERR_GENERAL (700)
// ERR_GENERAL_CHANNEL (701)

#endif
