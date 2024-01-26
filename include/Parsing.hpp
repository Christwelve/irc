# include "User.hpp"
# include <iostream>
# include <map>
# include <string>
# include <sstream>
# include <algorithm>
# include "Socket.hpp"
# include "User.hpp"

enum Command {
	QUIT,
	NICK,
	USER,
	JOIN,
	PART,
	PRIVMSG,
	KICK,
	INVITE,
	TOPIC,
	LIST,
	MODE
};

std::map<std::string, Command> commandMap;
void initializeCommandMap() {
	commandMap["QUIT"] = QUIT;
	commandMap["NICK"] = NICK;
	commandMap["USER"] = USER;
	commandMap["JOIN"] = JOIN;
	commandMap["PART"] = PART;
	commandMap["PRIVMSG"] = PRIVMSG;
	commandMap["KICK"] = KICK;
	commandMap["INVITE"] = INVITE;
	commandMap["TOPIC"] = TOPIC;
	commandMap["LIST"] = LIST;
	commandMap["MODE"] = MODE;
}


void quit(User &user, std::vector<std::string> tokens)
{

	
	std::cout << "quit() " << user.getNickname() << tokens[1] << std::endl;
	//sendFuntion()
	// :nickname!username@hostname QUIT :<quit message>\r\n
	
}	
	
// void nick(User &user, std::string newNickname)
void nick(User &user, std::vector<std::string> tokens)
{
	
	std::cout << "nick()" << std::endl;
}

// void userCmd(User &user, std::string rest)
void userCmd(User &user, std::vector<std::string> tokens)
{
		
	std::cout << "user() create or login a user" << std::endl;
}
// void join(User &user, std::string channel)
void join(User &user, std::vector<std::string> tokens)
{
	std::cout << "join()" << std::endl;
}
// void part(User &user, std::string channel)
void part(User &user, std::vector<std::string> tokens)
{
	std::cout << "part()" << std::endl;
}
// void privmsg(User &user, std::string reciever, std::string message)
void privmsg(User &user, std::vector<std::string> tokens)
{
	std::cout << "privmsg()" << std::endl;
}
// void privmsg(User &user, std::string channel, std::string message)
void privmsg(User &user, std::vector<std::string> tokens)
{
	std::cout << "privmsg()" << std::endl;
}
// void kick(User &user, std::string channel, std::string nickname, std::string reason)
void kick(User &user, std::vector<std::string> tokens)
{
std::cout << "kick()" << std::endl;
}
// void invite(User &user, std::string nickname, std::string channel)
void invite(User &user, std::vector<std::string> tokens)
{
	std::cout << "invite()" << std::endl;
}
// void topic(User &user, std::string channel, std::string newTopic)
void topic(User &user, std::vector<std::string> tokens)
{
	std::cout << "topic()" << std::endl;
}
void list() 
{
	std::cout << "list()" << std::endl;
}
// void mode(std::string channel, std::string mode)
void mode(std::vector<std::string> tokens)
{
	std::cout << "mode()" << std::endl;
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

	int commandValue = commandMap[tokens[0]];
	std::istringstream iss;
	std::string receiver, message;
	std::string channel, nickname, reason;
	switch (commandValue) {
		case QUIT:
			quit(user, tokens); 
			break;
		// case NICK:
		// 	nick(user, rest);
		// 	break;
		// case USER:
		// 	userCmd(user, rest);
		// 	break;
		// case JOIN:
		// 	join(user, rest);
		// 	break;
		// case PART:
		// 	part(user, rest);
		// 	break;
		// case PRIVMSG:
		// 	iss.str(rest);
		// 	iss >> receiver;
		// 	getline(iss, message);
		// 	message = message.substr(1);
		// 	privmsg(user, receiver, message);
		// 	break;
		// case KICK:
		// 	iss.str(rest);
		// 	iss >> channel >> nickname;
		// 	getline(iss, reason);
		// 	reason = reason.substr(1); 
		// 	kick(user, channel, nickname, reason);
		// 	break;
		// case INVITE:
		// 	iss.str(rest);
		// 	iss >> nickname >> channel;
		// 	invite(user, nickname, channel);
		// 	break;
		// case TOPIC:
		// 	iss.str(rest);
		// 	iss >> channel;
		// 	getline(iss, message);
		// 	message = message.substr(1); 
		// 	topic(user, channel, message);
		// 	break;
		// case LIST:
		// 	list();
		// 	break;
		// case MODE:
		// 	iss.str(rest);
		// 	iss >> channel >> message;
		// 	mode(channel, message);
		// 	break;            
		default:
			std::cout << ":server 421 client :Unknown command " << tokens[0] << std::endl;
			break;
	}
}

