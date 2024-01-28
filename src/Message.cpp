#include <sstream>
#include <algorithm>
#include <stdexcept>
#include "Message.hpp"

Message::Message(const std::string &input)
{
	std::string::size_type pos = input.find(':');
	std::string str = input;

    if (pos != std::string::npos)
	{
        trailing_ = str.substr(pos + 2);
        str = str.substr(0, pos);
    }

	std::stringstream stream(str);
	std::string param;

	stream >> command_;
	std::transform(command_.begin(), command_.end(), command_.begin(), ::toupper);

	while (stream >> param)
		params_.push_back(param);
}

Message::~Message() {}

bool Message::hasTrailing() const
{
	return (!trailing_.empty());
}

const std::string &Message::getCommand() const
{
	return (command_);
}

std::string Message::getParamAt(unsigned long index) const
{
	if(index >= params_.size())
		return ("");

	return (params_.at(index));
}

std::string Message::getTrailing() const
{
	return (hasTrailing() ? trailing_ : "");
}

size_t Message::getParamCount() const
{
	return (params_.size());
}
