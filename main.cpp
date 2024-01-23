
#include "Server.hpp"
#include "IRCError.hpp"
#include <sstream>


int parsing(int argc, char **argv) {
	if (argc != 3)
		throw IRCError("Use: ./irc <port> <password>");
	
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
