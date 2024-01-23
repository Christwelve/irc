#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define PORT 8080

int main()
{
	int server_fd, new_socket;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[BUFFER_SIZE] = {0};

	// Creating socket file descriptor
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Binding the socket to the port
	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if(listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	// Set the server socket to non-blocking
	fcntl(server_fd, F_SETFL, O_NONBLOCK);

	struct pollfd client_fds[MAX_CLIENTS];
	int nfds = 1;

	client_fds[0].fd = server_fd;
	client_fds[0].events = POLLIN;

	std::cout << "server listening on port " << PORT << std::endl;

	while (true)
	{
		int ret = poll(client_fds, nfds, -1);

		if(ret < 0)
		{
			perror("poll");
			exit(EXIT_FAILURE);
		}

		// Check for an incoming connection
		if(client_fds[0].revents & POLLIN)
		{
			if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
			{
				perror("accept");
				exit(EXIT_FAILURE);
			}

			// Set the new socket to non-blocking
			fcntl(new_socket, F_SETFL, O_NONBLOCK);

			std::cout << "new client connected " << new_socket << std::endl;

			// Add the new socket to the array
			client_fds[nfds].fd = new_socket;
			client_fds[nfds].events = POLLIN;
			nfds++;
		}

		// Check each client for data
		for (int i = 1; i < nfds; i++)
		{
			if(client_fds[i].revents & POLLIN)
			{
				memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
				ssize_t valread = recv(client_fds[i].fd, buffer, BUFFER_SIZE - 1, 0);

				if(valread < 0)
				{
					// Handle errors (e.g., client disconnection)
				}
				else
				{
					std::cout << "client " << client_fds[i].fd << ": " << buffer;
				}
			}
			else if(client_fds[i].revents & POLLOUT)
			{
				send(client_fds[i].fd, "hello\r\n", 7, 0);
			}

		}
	}

	return 0;
}
