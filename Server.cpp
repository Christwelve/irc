#include "Server.hpp"
#include "IRCError.hpp"

Server::Server(void)
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, signalHandler);
}

Server::~Server() {}

void Server::initServer(size_t port, const std::string &password)
{
	port_ = port;
	password_ = password;
	int server_fd;

	int opt = 1;

	// Creating socket file descriptor
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
		throw IRCError("Failed to initiate socket");

	// Forcefully attaching socket to the port
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw IRCError("Failed to set socket options");


	address_.sin_family = AF_INET;
	address_.sin_addr.s_addr = INADDR_ANY;
	address_.sin_port = htons(port_);

	// Binding the socket to the port
	if(bind(server_fd, (struct sockaddr *)&address_, sizeof(address_)) < 0)
		throw IRCError("Failed to bind socket to port");

	if(listen(server_fd, 3) < 0)
		throw IRCError("Failed to listen on socket");


	// Set the server socket to non-blocking
	fcntl(server_fd, F_SETFL, O_NONBLOCK);

	struct pollfd server = {server_fd, POLLIN, 0};
	clients_.push_back(server);


	std::cout << "server listening on port " << port_ << std::endl;
}

void Server::runServer(void)
{
	int new_socket;
	int addrlen = sizeof(this->address_);
	int test = 0;
	
	while (true)
	{
		struct pollfd &server = clients_.at(0);
		struct pollfd *fds = &server;
		int ret = poll(fds, clients_.size(), -1);

		if(ret < 0)
			throw IRCError("Failed to poll sockets");


		// Check for an incoming connection
		if(clients_.at(0).revents & POLLIN)
		{
			if((new_socket = accept(server.fd, (struct sockaddr *)&address_, (socklen_t*)&addrlen)) < 0)
				throw IRCError("Failed to accept socket connection");

			// Set the new socket to non-blocking
			fcntl(new_socket, F_SETFL, O_NONBLOCK);

			std::cout << "new client connected " << new_socket << std::endl;

			// Add the new socket to the array
			struct pollfd new_client = {new_socket, POLLIN | POLLOUT, 0};
			clients_.push_back(new_client);
		}

		// Check each client for data
		for (unsigned long i = 1; i < clients_.size(); i++)
		{
			struct pollfd client = clients_.at(i);

			if(client.revents & POLLIN)
			{
				memset(buffer_, 0, BUFFER_SIZE); // Clear the buffer
				ssize_t valread = recv(client.fd, buffer_, BUFFER_SIZE - 1, 0);

				if(valread < 0)
				{
					// Handle errors (e.g., client disconnection)
				}
				else if(valread == 0)
				{
					// Handle client disconnection
					std::cout << "client " << client.fd << " disconnected" << std::endl;
					close(client.fd);
					clients_.erase(clients_.begin() + i);
					i--;
				}
				else
				{
					std::cout << "client " << client.fd << ": " << buffer_;
					std::string msg(buffer_);

					if(msg.find("die") != std::string::npos)
						shutdownServer();
				}
			}
			else if(client.revents & POLLOUT && test == 0)
			{
				send(client.fd, "First message\r\n", 16, 0);
				test++;
			}
		}
	}
}

void Server::shutdownServer(void)
{
	std::cout << std::endl << "Shutting down server..." << std::endl;
	struct pollfd &server = clients_.at(0);
	int new_socket;
	int addrlen = sizeof(this->address_);
	while(server.revents & POLLIN)
	{
		if((new_socket = accept(server.fd, (struct sockaddr *)&address_, (socklen_t*)&addrlen)) < 0)
			continue;
		close(new_socket);
	}
	close(server.fd);
	for (unsigned long i = 1; i < clients_.size(); i++)
	{
		send(clients_.at(i).fd, "Fuck off\r\n", 10, 0);
		close(clients_.at(i).fd);
	}
	exit(EXIT_SUCCESS);
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

const char *IRCError::what() const throw()
{
	return errorMessage_;
}
