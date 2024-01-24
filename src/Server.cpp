#include "Server.hpp"
#include "IRCError.hpp"

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
	int test = 0;

	running_ = true;

	while (running_)
	{
		// redo this because poll modifies the struct but there is no reference to the struct in the socket, so my fancy functions don't work
		struct pollfd *fds = new struct pollfd[clients_.size() + 1];
		fds[0] = socket_.getPollFd();
		for (unsigned long i = 0; i < clients_.size(); i++)
			fds[i + 1] = clients_.at(i).getPollFd();
		int ret = poll(fds, clients_.size(), -1);

		if(ret < 0)
			throw IRCError("Failed to poll sockets");


		// Check for an incoming connection


		// Check each client for data
		for (unsigned long i = 1; i < clients_.size(); i++)
		{
			Socket client = clients_.at(i);

			if(client.hasPollIn())
			{
				char buffer[BUFFER_SIZE] = {0}; // Clear the buffer
				ssize_t valread = recv(client.getFd(), buffer, BUFFER_SIZE - 1, 0);

				if(valread < 0)
				{
					// Handle errors (e.g., client disconnection)
				}
				else if(valread == 0)
				{
					// Handle client disconnection
					std::cout << "client " << client.getFd() << " disconnected" << std::endl;
					client.close();
					clients_.erase(clients_.begin() + i);
					i--;
				}
				else
				{
					std::cout << "client " << client.getFd() << ": " << buffer;
					std::string msg(buffer);

					if(msg.find("die") != std::string::npos)
					{
						shutdownServer();
						running_ = false;
					}
				}
			}
			else if(client.hasPollOut() && test == 0)
			{
				client.send("First message");
				test = 1;
			}
		}
	}
}

void Server::listenForNewClients(void)
{
	if(!socket_.hasPollIn())
		return;

	Socket new_client(socket_.getFd(), socket_.getAddress());

	std::cout << "new client connected " << new_client.getFd() << std::endl;

	clients_.push_back(new_client);
}

void Server::shutdownServer(void)
{
	std::cout << std::endl << "Shutting down server..." << std::endl;

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

	running_ = false;
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
