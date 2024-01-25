#include "User.hpp"

User::User(const Socket &socket): socket_(socket), is_approved_(false), is_registered_(false), nickname_(""), username_(""), realname_(""), command_buffer_("") {}

User::~User() {}

Socket &User::getSocket(void)
{
	return (socket_);
}

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

bool User::operator==(const User &user) const
{
	return (socket_ == user.socket_);
}

bool User::operator!=(const User &user) const
{
	return (socket_ != user.socket_);
}
