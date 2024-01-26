#include "User.hpp"

User::User(const Socket &socket): socket_(socket), isApproved_(false), isRegistered_(false), nickname_(""), username_(""), realname_(""), commandBuffer_("") {}

User::~User() {}

Socket &User::getSocket(void)
{
	return (socket_);
}

bool User::isApproved(void) const
{
	return (isApproved_);
}

bool User::isRegistered(void) const
{
	return (isRegistered_);
}

void User::setApproved(void)
{
	isApproved_ = true;
}

void User::setRegistered(void)
{
	isRegistered_ = true;
}

const std::string &User::getNickname(void) const
{
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

void User::setNickname(const std::string &nickname)
{
	nickname_ = nickname;
}

void User::setUsername(const std::string &username)
{
	username_ = username;
}

void User::setRealname(const std::string &realname)
{
	realname_ = realname;
}

void User::appendCommandBuffer(const std::string &partialCommand)
{
	commandBuffer_ += partialCommand;
}

void User::queue(const std::string &message)
{
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

bool User::operator==(const User &user) const
{
	return (socket_ == user.socket_);
}

bool User::operator!=(const User &user) const
{
	return (socket_ != user.socket_);
}
