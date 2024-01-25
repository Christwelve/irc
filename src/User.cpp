#include "User.hpp"

User::User(int socket_fd): socket_fd_(socket_fd), is_approved_(false), is_registered_(false), nickname_(""), username_(""), realname_(""), command_buffer_("") {}

User::User(const User &user): socket_fd_(user.socket_fd_), is_approved_(user.is_approved_), is_registered_(user.is_registered_), nickname_(user.nickname_), username_(user.username_), realname_(user.realname_), command_buffer_(user.command_buffer_) {}

User &User::operator=(const User &user)
{
	if (this == &user)
		return (*this);

	socket_fd_ = user.socket_fd_;
	is_approved_ = user.is_approved_;
	is_registered_ = user.is_registered_;
	nickname_ = user.nickname_;
	username_ = user.username_;
	realname_ = user.realname_;
	command_buffer_ = user.command_buffer_;

	return (*this);
}

User::~User() {}

bool User::isApproved(void) const
{
	return (is_approved_);
}

bool User::isRegistered(void) const
{
	return (is_registered_);
}

void User::setApproved(void)
{
	is_approved_ = true;
}

void User::setRegistered(void)
{
	is_registered_ = true;
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

void User::appendCommandBuffer(const std::string &partial_command)
{
	command_buffer_ += partial_command;
}

Socket User::getSocketFd(void) const
{
	return (socket_fd_);
}
