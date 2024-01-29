#include "User.hpp"
#include "Server.hpp"
#include "ChannelManager.hpp"
#include "UserManager.hpp"

User::User(const Socket &socket): socket_(socket), state_(USER_EXPECTS_PASS), nickname_(""), username_(""), realname_(""), commandBuffer_("") {}

User::~User() {}

Socket &User::getSocket(void)
{
	return (socket_);
}

bool User::isRegistered(void) const
{
	return (state_ == USER_REGISTERED);
}

const std::string &User::getNickname(void) const
{
	static std::string asterix = "*";

	if (!isRegistered())
		return (asterix);

	return (nickname_);
}

const std::string &User::getUsername(void) const
{
	return (username_);
}

const std::string &User::getRealname(void) const
{
	return (realname_);
}

std::string User::getUserIdent(void) const
{
	return (nickname_ + "!" + username_ + "@" + socket_.getIp());
}

bool User::hasState(UserState state) const
{
	return (state_ == state);
}

void User::setState(UserState state)
{
	state_ = state;
}

void User::setNickname(const std::string &nickname)
{
	nickname_ = nickname;
}

void User::setUsername(const std::string &username, const std::string &realname)
{
	username_ = username;
	realname_ = realname;
}

void User::appendCommandBuffer(const std::string &partialCommand)
{
	commandBuffer_ += partialCommand;
}

void User::queue(const std::string &message)
{
	std::cout << "TO " << nickname_ << " (" << socket_.getFd() << "): " << message << std::endl;
	messageQueue_.push(message + "\r\n");
}


std::string &User::getNextMessage(void)
{
	return (messageQueue_.front());
}

void User::finishedSendingMessage(void)
{
	messageQueue_.pop();
}

bool User::hasInput(void) const
{
	if(commandBuffer_.find("\r\n") != std::string::npos)
		return (true);
	if(commandBuffer_.find("\n") != std::string::npos)
		return (true);
	return (false);
}

bool User::hasOutput(void) const
{
	return (!messageQueue_.empty());
}

std::string User::getInputFromCommandBuffer(void)
{
	std::string input = commandBuffer_;

	if(input.find("\r\n") != std::string::npos)
	{
		input.erase(input.find("\r\n"));
		commandBuffer_.erase(0, input.length() + 2);
	}
	else if(input.find("\n") != std::string::npos)
	{
		input.erase(input.find("\n"));
		commandBuffer_.erase(0, input.length() + 1);
	}
	return (input);
}

void User::remove(void)
{
	ChannelManager::getInstance().removeUserFromAllChannels(*this);
	UserManager::getInstance().removeUser(*this);
}

bool User::operator==(const User &user) const
{
	return (socket_ == user.socket_);
}

bool User::operator!=(const User &user) const
{
	return (socket_ != user.socket_);
}
