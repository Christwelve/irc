# include "User.hpp"
# include <iostream>
# include <map>
# include <string>
# include <sstream>
# include <algorithm>
# include "Socket.hpp"
# include "User.hpp"


void quit(User &user, std::vector<std::string> tokens)
{
	(void)user;
	std::cout << "quit() " << user.getNickname() << tokens[1] << std::endl;
	//sendFuntion()
	// :nickname!username@hostname QUIT :<quit message>\r\n
	
}	
	
void nick(User &user, std::vector<std::string> tokens)
{
	(void)user;
	std::cout << "nick() " << tokens[1] << std::endl;
}

void userCmd(User &user, std::vector<std::string> tokens)
{
	(void)user;
	std::cout << "user() "  << tokens[1] << tokens[2]  << tokens[3] << tokens[4] << std::endl;
}

void join(User &user, std::vector<std::string> tokens)
{
	(void)user;
	std::cout << "join() " << tokens[1] << std::endl;
}

void part(User &user, std::vector<std::string> tokens)
{
	(void)user;

	std::cout << "part() " << tokens[1] << std::endl;
}

void privmsg(User &user, std::vector<std::string> tokens)
{
	(void)user;
	std::cout << "privmsg() " << tokens[1] << tokens[2] << std::endl;
}

void kick(User &user, std::vector<std::string> tokens)
{
	(void)user;
	std::cout << "kick() " << tokens[1] << tokens[2] << tokens[3] << std::endl;
}

void invite(User &user, std::vector<std::string> tokens)
{
	(void)user;
	std::cout << "invite() " << tokens[1] << tokens[2] << std::endl;
}

void topic(User &user, std::vector<std::string> tokens)
{
	(void)user;

	std::cout << "topic() " << tokens[1] << tokens[2] << std::endl;
}
void list(User &user, std::vector<std::string> tokens) 
{
	(void)user;
	(void)tokens;
	std::cout << "list() " << std::endl;
}

void mode(User &user, std::vector<std::string> tokens)
{
	(void)user;

	std::cout << "mode() " << tokens[1] << tokens[2] << std::endl;
}

typedef void (*CommandFunc)(User&, std::vector<std::string>);
std::map<std::string, CommandFunc> commandMap;
void initializeCommandMap() {
	commandMap["QUIT"] = quit;
	commandMap["NICK"] = nick;
	commandMap["USER"] = userCmd;
	commandMap["JOIN"] = join;
	commandMap["PART"] = part;
	commandMap["PRIVMSG"] = privmsg;
	commandMap["KICK"] = kick;
	commandMap["INVITE"] = invite;
	commandMap["TOPIC"] = topic;
	commandMap["LIST"] = list;
	commandMap["MODE"] = mode;
}

std::string addSpaceAfterColon(std::string str) {
    size_t pos = str.find(':');
    if (pos != std::string::npos && pos < str.size() - 1 && str[pos + 1] != ' ') {
        str.insert(pos + 1, " ");
    }
    return str;
}

void inputParsing(User &user, std::string input)
{

	input = addSpaceAfterColon(input);

	std::string::size_type pos = input.find(':');
    std::string temp;
    if (pos != std::string::npos) {
        temp = input.substr(pos + 2); 
        input = input.substr(0, pos); 
    }

	std::stringstream ss(input);
	std::vector<std::string> tokens;
	std::string token;
	while (ss >> token)
		tokens.push_back(token);	
	tokens.push_back(temp);

	std::transform(tokens[0].begin(), tokens[0].end(), tokens[0].begin(), ::toupper);

    std::map<std::string, CommandFunc>::iterator it = commandMap.find(tokens[0]);
    if (it != commandMap.end()) {
        it->second(user, tokens);
    } else {
        std::cout << ":server 421 client :Unknown command " << tokens[0] << std::endl;
	}
}
