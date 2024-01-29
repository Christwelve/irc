#include <errno.h>
#include <sstream>
#include <arpa/inet.h>
#include "Server.hpp"
#include "IRCError.hpp"
#include "UserManager.hpp"
#include "Parsing.hpp"

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
				std::cout << "Failed to send message to socket " << socket.getFd() << std::endl;
				user.remove();
				continue;
			}
			else if(valread == 0)
			{
				// Handle socket disconnection
				std::cout << "socket " << socket.getFd() << " disconnected" << std::endl;
				user.remove();
				i--;
				continue;
			}
			else
			{
				std::string msg(buffer);

				user.appendCommandBuffer(msg);

				if(user.hasInput())
				{
					std::string input = user.getInputFromCommandBuffer();

					if(input.length() == 0)
						continue;

					std::cout << "FROM " << socket.getFd() << ": " << input << std::endl;

					parseInput(user, msg);
				}
				else
				{
					std::cout << "received partial from socket " << socket.getFd() << ": " << msg << std::endl;
				}
			}
		}
		if(socket.hasPollOut())
		{
			if(!user.hasOutput())
				continue;

			std::string &message = user.getNextMessage();
			errno = 0;
			ssize_t sent = send(socket.getFd(), message.c_str(), message.length(), 0);

			if(errno != EWOULDBLOCK && errno != EAGAIN && errno != 0)
				throw IRCError("Failed to send message to socket " + std::to_string(socket.getFd()));

			if(sent == -1)
			{
				std::cerr << "Failed to send message to socket " << socket.getFd() << std::endl;
				user.remove();
				continue;
			}

			message.erase(0, sent);
			if(message.length() == 0)
				user.finishedSendingMessage();
		}
	}
}

void Server::shutdownServer(void)
{
	std::cout << std::endl << "Shutting down server..." << std::endl;

	running_ = false;

	while(socket_.hasPollIn())
	{
		Socket new_client(socket_.getFd(), true);
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

bool Server::isPasswordValid(const std::string &password) const
{
	return (password == password_);
}

Server &Server::getInstance(void)
{
	static Server instance;

	return (instance);
}
