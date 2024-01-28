#ifndef MESSAGEDEFINES_HPP
# define MESSAGEDEFINES_HPP

# include <string>
# include "Server.hpp"

# define PONG_WITH_TRAILING(message) ("PONG " + std::string(SERVER_NAME) + " :" + message.getTrailing())
# define PONG_WITH_PARAM(message) ("PONG " + std::string(SERVER_NAME) + " " + message.getParamAt(0))
# define NICK_CHANGED(user, nickname) (":" + user.getUserIdent() + " NICK :" + nickname)
# define USER_CHANGED(user, username, hostname, servername, realname) (":" + user.getUserIdent() + " USER " + username + " " + hostname + " " + servername + " :" + realname)
# define JOIN_CHANNEL(user, channelName) (":" + user.getUserIdent() + " JOIN " + channelName)
# define PART_CHANNEL(user, channelName, comment) (":" + user.getUserIdent() + " PART " + channelName + " :" + comment)
# define PRIVMSG_SEND_MESSAGE(user, targetName, message) (":" + user.getUserIdent() + " PRIVMSG " + targetName + " :" + message)
# define KICK_USER(user, channelName, targetName, comment) (":" + user.getUserIdent() + " KICK " + channelName + " " + targetName + " :" + comment)
# define INVITE_USER(user, targetName, channelName) (":" + user.getUserIdent() + " INVITE " + targetName + " " + channelName)
# define TOPIC_CHANGED(user, channelName, topic) (":" + user.getUserIdent() + " TOPIC " + channelName + " :" + topic)
# define LIST_CHANNELS(user, channelName) (":" + user.getUserIdent() + " LIST " + channelName)
# define MODE_CHANNEL(user, channelName, modes) (":" + user.getUserIdent() + " MODE " + channelName + " " + modes)
# define MODE_INVITE_ONLY(user, channelName, mode) (":" + user.getUserIdent() + " MODE " + channelName + " " + mode)
# define MODE_TOPIC_RESTRICTED(user, channelName, mode) (":" + user.getUserIdent() + " MODE " + channelName + " " + mode)
# define MODE_SET_KEY_REQUIRED(user, channelName, mode, key) (":" + user.getUserIdent() + " MODE " + channelName + " " + mode + "" + key)
# define MODE_SET_OPERATOR(user, channelName, mode, targetName) (":" + user.getUserIdent() + " MODE " + channelName + " " + mode + " " + targetName)
# define MODE_SET_USER_LIMIT(user, channelName, mode, limit) (":" + user.getUserIdent() + " MODE " + channelName + " " + mode + " " + limit)
# define MODE_UNSET_KEY_REQUIRED(user, channelName, mode) (":" + user.getUserIdent() + " MODE " + channelName + " " + mode)
# define MODE_UNSET_OPERATOR(user, channelName, mode, targetName) (":" + user.getUserIdent() + " MODE " + channelName + " " + mode + " " + targetName)
# define MODE_UNSET_USER_LIMIT(user, channelName, mode) (":" + user.getUserIdent() + " MODE " + channelName + " " + mode)

# define USER_WELCOME(user) (":" + std::string(SERVER_NAME) + " 001 " + user.getNickname() + " :Ey Mate! Welcome to 👁️  ®️  🌊")
# define ERR_NICK_ALREADY_IN_USE(user, nickname) (":" + std::string(SERVER_NAME) + " 433 " + user.getNickname() + " " + nickname + " :Nickname is already in use")
# define ERR_USER_DOESNT_EXIST(user, nickname) (":" + std::string(SERVER_NAME) + " 401 " + user.getNickname() + " " + nickname + " :This nickname doesn't exist")
# define ERR_USER_NOT_REGISTERED(user) (":" + std::string(SERVER_NAME) + " 451 " + user.getNickname() + " :You have not registered")
# define ERR_USER_ALREADY_REGISTERED(user) (":" + std::string(SERVER_NAME) + " 462 " + user.getNickname() + " :Already registered")
# define ERR_USER_ALREADY_OPERATOR(user, channelName, targetName) (":" + std::string(SERVER_NAME) + " 512 " + user.getNickname() + " " + channelName + " " + targetName + " :is already channel operator")
# define ERR_CHANNEL_INVALID_NAME(user, channelName) (":" + std::string(SERVER_NAME) + " 480 " + user.getNickname() + " " + channelName + " :Invalid Channel Name")
# define ERR_CHANNEL_INVALID_KEY(user, channelName) (":" + std::string(SERVER_NAME) + " 479 " + user.getNickname() + " " + channelName + " :Invalid Channel Key")
# define ERR_NOT_MEMBER_OF_CHANNEL(user, channelName) (":" + std::string(SERVER_NAME) + " 442 " + user.getNickname() + " " + channelName + " :You're not a member of this channel")
# define ERR_CHANNEL_DOESNT_EXIST(user, channelName) (":" + std::string(SERVER_NAME) + " 403 " + user.getNickname() + " " + channelName + " :Channel doesn't exist")
# define ERR_USER_NOT_OPERATOR(user, channelName) (":" + std::string(SERVER_NAME) + " 482 " + user.getNickname() + " " + channelName + " :You're not channel operator")
# define ERR_CHANNEL_INVITE_ONLY(user, channelName) (":" + std::string(SERVER_NAME) + " 473 " + user.getNickname() + " " + channelName + " :Cannot join channel (+i)")
# define ERR_USER_NOT_IN_CHANNEL(user, channelName) (":" + std::string(SERVER_NAME) + " 441 " + user.getNickname() + " " + channelName + " :User isn't on that channel")
# define ERR_USER_IS_ALREADY_ON_CHANNEL(user, channelName) (":" + std::string(SERVER_NAME) + " 443 " + user.getNickname() + channelName + ":User is already on that channel")
# define ERR_CHANNEL_IS_FULL(user, channelName) (":" + std::string(SERVER_NAME) + " 471 " + user.getNickname() + " " + channelName + " :Channel is full")


# define ERR_PRIVMSG_NO_TEXT_TO_SEND(user) (":" + std::string(SERVER_NAME) + " 412 " + user.getNickname() + " :No text to send")

# define ERR_NEED_MORE_PARAMS(user, command) (":" + std::string(SERVER_NAME) + " 461 " + user.getNickname() + " " + command + " :Not enough parameters")
# define ERR_UNKNOWN_COMMAND(user, command) (":" + std::string(SERVER_NAME) + " 421 " + user.getNickname() + " " + command + " :Unknown command")
# define ERR_INVALID_PARAM(user, param) (":" + std::string(SERVER_NAME) + " 421 " + user.getNickname() + " " + param + " :Invalid parameter")

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
