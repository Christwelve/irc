#ifndef MESSAGEDEFINES_HPP
# define MESSAGEDEFINES_HPP

# include <string>
# include "Server.hpp"

# define PONG_WITH_TRAILING(message) ("PONG " + std::string(SERVER_NAME) + " :" + message.getTrailing())
# define PONG_WITH_PARAM(message) ("PONG " + std::string(SERVER_NAME) + " " + message.getParamAt(0))

# define NICK_CHANGED(user, nickname) (":" + user.getUserIdent() + " NICK :" + nickname)

# define PRIVMSG_SEND_MESSAGE(user, target, message) (":" + user.getUserIdent() + " PRIVMSG " + target + " :" + message)




# define ERR_NICK_ALREADY_IN_USE(user, nickname) (":" + std::string(SERVER_NAME) + " 433 " + user.getNickname() + " " + nickname + " :Nickname is already in use")
# define ERR_NICK_DOESNT_EXIST(user, nickname) (":" + std::string(SERVER_NAME) + " 401 " + user.getNickname() + " " + nickname + " :This nickname doesn't exist")
# define ERR_USER_ALREADY_REGISTERED(user) (":" + std::string(SERVER_NAME) + " 462 " + user.getNickname() + " :Already registered")
# define ERR_CHANNEL_INVALID_NAME(user, channel) (":" + std::string(SERVER_NAME) + " 480 " + user.getNickname() + " " + channel + " :Invalid Channel Name")
# define ERR_CHANNEL_INVALID_KEY(user, channel) (":" + std::string(SERVER_NAME) + " 479 " + user.getNickname() + " " + channel + " :Invalid Channel Key")
# define ERR_NOT_MEMBER_OF_CHANNEL(user, channel) (":" + std::string(SERVER_NAME) + " 442 " + user.getNickname() + " " + channel + " :You're not a member of this channel")
# define ERR_CHANNEL_DOESNT_EXIST(user, channel) (":" + std::string(SERVER_NAME) + " 403 " + user.getNickname() + " " + channel + " :Channel doesn't exist")

# define ERR_PRIVMSG_NO_TEXT_TO_SEND(user) (":" + std::string(SERVER_NAME) + " 412 " + user.getNickname() + " :No text to send")
# define ERR_PRIVMSG_NOT_ON_CHANNEL(user, channel) (":" + std::string(SERVER_NAME) + " 442 " + user.getNickname() + " " + channel + " :You're not on that channel")

# define ERR_NEED_MORE_PARAMS(user, command) (":" + std::string(SERVER_NAME) + " 461 " + user.getNickname() + " " + command + " :Not enough parameters")
# define ERR_UNKNOWN_COMMAND(user, command) (":" + std::string(SERVER_NAME) + " 421 " + user.getNickname() + " " + command + " :Unknown command")

#endif
