#include "Server.hpp"
#include "IRCError.hpp"
#include "UserManager.hpp"

Server::Server(void)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signalHandler);

	running_ = false;
}

Server::~Server() {}

void Server::initServer(int port, const std::string &password)
{
	password_ = password;
	socket_ = Socket(port);

	std::cout << "server listening on port " << port << std::endl;
}

void Server::runServer(void)
{
	running_ = true;

	while (running_)
	{
		pollSockets();
		listenForNewClients();
		processClientSockets();
	}
}

void Server::pollSockets(void)
{
	UserManager &userManager = UserManager::getInstance();
	struct pollfd *fds = userManager.getPollFdsWithServerSocket(socket_);

	int pollResult = poll(fds, userManager.getUserCount() + 1, -1);

	if(pollResult == -1)
		throw IRCError("Failed to poll sockets");

	userManager.setPollFdsWithServerSocket(socket_, fds);

	delete[] fds;
}

void Server::listenForNewClients(void)
{
	if(!socket_.hasPollIn())
		return;

	Socket new_client(socket_.getFd(), socket_.getAddress());

	std::cout << "new client connected " << new_client.getFd() << std::endl;

	UserManager::getInstance().createUserFromSocket(new_client);
}

void Server::processClientSockets(void)
{
	UserManager &userManager = UserManager::getInstance();
	std::vector<User> &users = userManager.getUsers();

	for (unsigned long i = 0; i < userManager.getUserCount(); i++)
	{
		User &user = users.at(i);
		Socket &socket = user.getSocket();

		if(socket.hasPollIn())
		{
			char buffer[BUFFER_SIZE] = {0}; // Clear the buffer
			ssize_t valread = recv(socket.getFd(), buffer, BUFFER_SIZE - 1, 0);

			if(valread < 0)
			{
				// Handle errors (e.g., socket disconnection)
			}
			else if(valread == 0)
			{
				// Handle socket disconnection
				std::cout << "socket " << socket.getFd() << " disconnected" << std::endl;
				userManager.removeUser(user);
				i--;
			}
			else
			{
				std::string msg(buffer);
				while(msg.find("\n") != std::string::npos)
					msg.erase(msg.find("\n"));

				std::cout << "socket " << socket.getFd() << ": " << msg << std::endl;

				if(msg.find("die") != std::string::npos)
				{
					shutdownServer();
					running_ = false;
				}
			}
		}
		if(socket.hasPollOut())
		{
			// client.send("First message");
			// test = 1;
		}
	}
}

void Server::shutdownServer(void)
{
	std::cout << std::endl << "Shutting down server..." << std::endl;

	running_ = false;

	while(socket_.hasPollIn())
	{
		Socket new_client(socket_.getFd(), socket_.getAddress());
		new_client.close();
	}

	socket_.close();

	for (std::vector<Socket>::iterator it = clients_.begin(); it != clients_.end(); it++)
	{
		it->send("Server shutting down");
		it->close();
	}
}
void Server::signalHandler(int signal)
{
	(void) signal;

	Server &server = Server::getInstance();

	server.shutdownServer();
}

Server &Server::getInstance(void)
{
	static Server instance;

	return (instance);
}
