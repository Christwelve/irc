# include "Server.hpp"
# include <iostream>
# include <map>
# include <string>
# include <sstream>
# include <algorithm>
# include "User.hpp"
# include "Socket.hpp"
# include "User.hpp"
# include "UserManager.hpp"

#include <string>
#include <map>


UserManager& userManager = UserManager::getInstance();

std::string serverToClient(User &user, int status, std::string command) {
    std::map<int, std::string> statusMap;
    statusMap[001] = ":" + std::string(SERVER_NAME) + " 001 " + user.getNickname() + " :Welcome message sent after a successful connection.";
    // statusMap[002] = ":" + std::string(SERVER_NAME) + " 002 " + user.getNickname() + " :Server's version information.";
    // statusMap[003] = ":" + std::string(SERVER_NAME) + " 003 " + user.getNickname() + " :Server's creation date.";
    // statusMap[004] = ":" + std::string(SERVER_NAME) + " 004 " + user.getNickname() + " :Server's details (name, version, available user modes, available channel modes).";
    // statusMap[005] = ":" + std::string(SERVER_NAME) + " 005 " + user.getNickname() + " :Additional server features.";
    // statusMap[353] = ":" + std::string(SERVER_NAME) + " 353 " + user.getNickname() + " :Reply to the NAMES command, listing all visible users in a channel.";
    // statusMap[366] = ":" + std::string(SERVER_NAME) + " 366 " + user.getNickname() + " :End of NAMES list.";
    // statusMap[372] = ":" + std::string(SERVER_NAME) + " 372 " + user.getNickname() + " :MOTD (Message of the Day) text line.";
    // statusMap[375] = ":" + std::string(SERVER_NAME) + " 375 " + user.getNickname() + " :Start of MOTD.";
    // statusMap[376] = ":" + std::string(SERVER_NAME) + " 376 " + user.getNickname() + " :End of MOTD.";
    statusMap[421] = ":" + std::string(SERVER_NAME) + " 421 " + user.getNickname() + " :Unknown command.";
    statusMap[433] = ":" + std::string(SERVER_NAME) + " 433 " + user.getNickname() + " :Nickname is already in use.";
    statusMap[441] = ":" + std::string(SERVER_NAME) + " 441 " + user.getNickname() + " :User not in channel (used when trying to kick a user not in channel).";
    statusMap[442] = ":" + std::string(SERVER_NAME) + " 442 " + user.getNickname() + " :Not on channel (used when trying to perform a channel operation without being on the channel).";
    statusMap[461] = ":" + std::string(SERVER_NAME) + " 461 " + user.getNickname() + " " + command + " :Need more parameters.";
    statusMap[462] = ":" + std::string(SERVER_NAME) + " 462 " + user.getNickname() + " :Already registered (used when a client tries to register again).";
    statusMap[464] = ":" + std::string(SERVER_NAME) + " 464 " + user.getNickname() + " :Incorrect password.";
    statusMap[465] = ":" + std::string(SERVER_NAME) + " 465 " + user.getNickname() + " :You are banned from this server.";
    statusMap[471] = ":" + std::string(SERVER_NAME) + " 471 " + user.getNickname() + " :Channel is full.";
    statusMap[473] = ":" + std::string(SERVER_NAME) + " 473 " + user.getNickname() + " :Invite only channel.";
    statusMap[474] = ":" + std::string(SERVER_NAME) + " 474 " + user.getNickname() + " :Banned from channel.";
    statusMap[475] = ":" + std::string(SERVER_NAME) + " 475 " + user.getNickname() + " :Bad channel key.";

    return statusMap[status];
}

void ping(User &user, std::vector<std::string> tokens)
{
	std::string out;
	if (tokens.size() == 1)
		out = serverToClient(user, 461, "PING"); 										//no origin specified
	else if (tokens.size() == 2)
		out = std::string(":") + " PONG " + SERVER_NAME + " " + tokens[1];				//pong
	else
		out = ":" + std::string(SERVER_NAME) + " PONG " + tokens[1] + " " + tokens[2];	//pong
	user.queue(out);
}

void cap(User &user, std::vector<std::string> tokens)
{
	(void)user;
	(void)tokens;
	return;	
}	

void quit(User &user, std::vector<std::string> tokens)
{
	(void)user;
	std::cout << "quit() " << user.getNickname() << tokens[1] << std::endl;
	//sendFuntion()
	// :nickname!username@hostname QUIT :<quit message>\r\n
	
}	
	
void nick(User &user, std::vector<std::string> tokens)
{
	std::string out;
	if (!tokens[1].size()) 
		out = serverToClient(user, 431); 												//no nickname given
	else if (userManager.getUserByNickname(tokens[1]) != userManager.getUsers().end()) 
		out = serverToClient(user, 433); 												//nickname already in use
	else if (user.isRegistered()) 
	{
		out = ":" + user.getNickname() + " NICK :" + tokens[1];
		user.setNickname(tokens[1]);													// set new nickname
	} 
	else 
	{
		userManager.createUserFromSocket(user.getSocket());
		out = ":" + user.getNickname() + " NICK :" + tokens[1];							//create new user
	}
	user.queue(out);
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
	commandMap["PING"] = ping;
	commandMap["CAP" ] = cap;
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
        std::string out = serverToClient(user, 421); //unknown command
	}
}
