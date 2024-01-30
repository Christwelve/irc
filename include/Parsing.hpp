#ifndef PARSING_HPP
# define PARSING_HPP

#include <string>
#include <map>

#include "User.hpp"
#include "Message.hpp"


typedef std::string (*CommandFunc)(User&, const Message&);

class Parsing
{


	public:
		Parsing(User &user, std::string input);	// Constructor
		~Parsing();								// Destructor

		// void parseInput(User &user, std::string input);
		std::map<std::string, CommandFunc> getCommandMap();

	private:

};

#endif
