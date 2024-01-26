# include "Server.hpp"
# include "IRCError.hpp"
# include "Parsing.hpp"
# include "User.hpp"
# include "Socket.hpp"
# include <sstream>


// int parsing(int argc, char **argv) {
// 	if (argc != 3)
// 		throw IRCError("Usage: ./irc <port> <password>");

// 	std::stringstream ss(argv[1]);
// 	int port;

// 	if (!(ss >> port) || port < 0 || port > 65535 || !ss.eof())
// 		throw IRCError(std::string("Invalid port number ") + argv[1]);

// 	return (port);
// }

// int main(int argc, char **argv)
// {
	// try
	// {
	// 	int port = parsing(argc, argv);

	// 	std::cout << "port: " << port << std::endl;
	// 	std::cout << "password: " << argv[2] << std::endl;
	// 	Server &server = Server::getInstance();
	// 	server.initServer(port, argv[2]);
	// 	server.runServer();
	// }
	// catch (const std::exception &e)
	// {
	// 	std::cerr << e.what() << std::endl;
	// 	return (1);
	// }
	// return (0);
// }

int main(void) {
	Socket socket;
	User user(socket);

    initializeCommandMap();

    // Test the inputParsing function with some test inputs
	std::cout << "Test 1" << std::endl;
    inputParsing(user, "qUiT :quitmessage");
	// std::cout << "Test 2" << std::endl;
    // inputParsing(user, "QUIT : \r\n");
	// std::cout << "Test 3" << std::endl;
    // inputParsing(user, "QUIT \r\n");
	// std::cout << "Test 4" << std::endl;
    // inputParsing(user, "QUI\r\n");
    // inputParsing(user, "NICK newNickname\r\n");
    // inputParsing(user, "USER username hostname servername :realname\r\n");
    // inputParsing(user, "JOIN #channel\r\n");
    // inputParsing(user, "PART #channel\r\n");
    // inputParsing(user, "PRIVMSG receiver :message\r\n");
    // inputParsing(user, "KICK #channel nickname :reason\r\n");
    // inputParsing(user, "INVITE nickname #channel\r\n");
    // inputParsing(user, "TOPIC #channel :newTopic\r\n");
    // inputParsing(user, "LIST\r\n");
    // inputParsing(user, "MODE #channel +o\r\n");

    return (0);
}
