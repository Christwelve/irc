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
	std::string token;

	stream >> command_;
	std::transform(command_.begin(), command_.end(), command_.begin(), ::toupper);
	
	while (stream >> token)
		params_.push_back(token);	
}

Message::~Message() {}

bool Message::hasTrailing() const
{
	return !trailing_.empty();
}

const std::string &Message::getCommand() const
{
	return (command_);
}

const std::string &Message::getParamAt(unsigned long index) const
{
	return params_.at(index);
}

const std::string &Message::getTrailing() const
{
	if (!hasTrailing())
		throw std::out_of_range("No trailing");
	return trailing_;
}

size_t Message::getParamCount() const
{
	return params_.size();
}
