# include "Server.hpp"
# include "IRCError.hpp"
# include "Parsing.hpp"
# include "User.hpp"
# include "Socket.hpp"
# include <sstream>


int parsing(int argc, char **argv) {
	if (argc != 3)
		throw IRCError("Usage: ./irc <port> <password>");

	std::stringstream ss(argv[1]);
	int port;

	if (!(ss >> port) || port < 0 || port > 65535 || !ss.eof())
		throw IRCError(std::string("Invalid port number ") + argv[1]);

	return (port);
}

int main(int argc, char **argv)
{
	try
	{
		int port = parsing(argc, argv);

		std::cout << "port: " << port << std::endl;
		std::cout << "password: " << argv[2] << std::endl;
		Server &server = Server::getInstance();
		server.initServer(port, argv[2]);
		server.runServer();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}

// int main(void) {
// 	Socket socket;
// 	User user(socket);

//     initializeCommandMap();


//     inputParsing(user, "qUiT :quitmessage");
//     inputParsing(user, "NICK newNickname");
//     inputParsing(user, "USER username hostname servername :realname");
//     inputParsing(user, "JOIN #channel");
//     inputParsing(user, "PART #channel");
//     inputParsing(user, "PRIVMSG receiver :message");
//     inputParsing(user, "PRIVMSG #channel :message");
//     inputParsing(user, "KICK #channel nickname :reason");
//     inputParsing(user, "INVITE nickname #channel");
//     inputParsing(user, "TOPIC #channel :newTopic");
//     inputParsing(user, "LIST");
//     inputParsing(user, "MODE #channel +o");

//     return (0);
// }
