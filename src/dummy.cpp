// #include <iostream>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <fcntl.h>
// #include <unistd.h>
// #include <poll.h>
// #include <vector>
// #include <signal.h>


// #define MAX_CLIENTS 10
// #define BUFFER_SIZE 1024
// #define PORT 8080


// signal(SIGINT, shutdown_server(server_fd, clients));
// void shutdownServer(int server_fd, std::vector<struct pollfd> clients)
// {

// 	while (clients.at(0).revents & POLLIN) 
// 		close(clients.at(0).fd);
// 	for (unsigned long i = 1; i < clients.size(); i++)
// 	{
// 		close(clients.at(i).fd);
// 	}
// 	close(server_fd);
// 	exit(EXIT_SUCCESS);
// } 

// int main()
// {
// 	signal(SIGPIPE, SIG_IGN);

// 	int server_fd, new_socket;
// 	struct sockaddr_in address;
// 	int opt = 1;
// 	int addrlen = sizeof(address);
// 	char buffer[BUFFER_SIZE] = {0};

// 	// Creating socket file descriptor
// 	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
// 	{
// 		perror("socket failed");
// 		exit(EXIT_FAILURE);
// 	}

// 	// Forcefully attaching socket to the port
// 	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
// 	{
// 		perror("setsockopt");
// 		exit(EXIT_FAILURE);
// 	}

// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = INADDR_ANY;
// 	address.sin_port = htons(PORT);

// 	// Binding the socket to the port
// 	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
// 	{
// 		perror("bind failed");
// 		exit(EXIT_FAILURE);
// 	}

// 	if(listen(server_fd, 3) < 0)
// 	{
// 		perror("listen");
// 		exit(EXIT_FAILURE);
// 	}

// 	// Set the server socket to non-blocking
// 	fcntl(server_fd, F_SETFL, O_NONBLOCK);

// 	std::vector<struct pollfd> clients;

// 	struct pollfd server = {server_fd, POLLIN, 0};
// 	clients.push_back(server);


// 	std::cout << "server listening on port " << PORT << std::endl;
	
// 	int new_socket;
// 	int addrlen = sizeof(this->address);
// 	while (true)
// 	{
// 		struct pollfd *fds = &clients.at(0);
// 		int ret = poll(fds, clients.size(), -1);

// 		if(ret < 0)
// 		{
// 			perror("poll");
// 			exit(EXIT_FAILURE);
// 		}

// 		// Check for an incoming connection
// 		if(clients.at(0).revents & POLLIN)
// 		{
// 			if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
// 			{
// 				perror("accept");
// 				exit(EXIT_FAILURE);
// 			}

// 			// Set the new socket to non-blocking
// 			fcntl(new_socket, F_SETFL, O_NONBLOCK);

// 			std::cout << "new client connected " << new_socket << std::endl;

// 			// Add the new socket to the array
// 			struct pollfd new_client = {new_socket, POLLIN | POLLOUT, 0};
// 			clients.push_back(new_client);
// 		}

// 		// Check each client for data
// 		for (unsigned long i = 1; i < clients.size(); i++)
// 		{
// 			struct pollfd client = clients.at(i);

// 			if(client.revents & POLLIN)
// 			{
// 				memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
// 				ssize_t valread = recv(client.fd, buffer, BUFFER_SIZE - 1, 0);

// 				if(valread < 0)
// 				{
// 					// Handle errors (e.g., client disconnection)
// 				}
// 				else if(valread == 0)
// 				{
// 					// Handle client disconnection
// 					std::cout << "client " << client.fd << " disconnected" << std::endl;
// 					close(client.fd);
// 					clients.erase(clients.begin() + i);
// 					i--;
// 				}
// 				else
// 				{
// 					std::cout << "client " << client.fd << ": " << buffer;
// 				}
// 			}
// 			else if(client.revents & POLLOUT)
// 			{
// 				send(client.fd, "hello\r\n", 7, 0);
// 			}

// 		}
// 	}

// 	return 0;
// }


// // #define MAX_CLIENTS 10
// #define BUFFER_SIZE 1024
// #define PORT 8080


// // signal(SIGINT, shutdown_server(server_fd, clients));
// void shutdownServer(int server_fd, std::vector<struct pollfd> clients)
// {

// 	while (clients.at(0).revents & POLLIN) 
// 		close(clients.at(0).fd);
// 	for (unsigned long i = 1; i < clients.size(); i++)
// 	{
// 		close(clients.at(i).fd);
// 	}
// 	close(server_fd);
// 	exit(EXIT_SUCCESS);
// } 

// int main()
// {
// 	signal(SIGPIPE, SIG_IGN);

// 	int server_fd, new_socket;
// 	struct sockaddr_in address;
// 	int opt = 1;
// 	int addrlen = sizeof(address);
// 	char buffer[BUFFER_SIZE] = {0};

// 	// Creating socket file descriptor
// 	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
// 	{
// 		perror("socket failed");
// 		exit(EXIT_FAILURE);
// 	}

// 	// Forcefully attaching socket to the port
// 	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
// 	{
// 		perror("setsockopt");
// 		exit(EXIT_FAILURE);
// 	}

// 	address.sin_family = AF_INET;
// 	address.sin_addr.s_addr = INADDR_ANY;
// 	address.sin_port = htons(PORT);

// 	// Binding the socket to the port
// 	if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
// 	{
// 		perror("bind failed");
// 		exit(EXIT_FAILURE);
// 	}

// 	if(listen(server_fd, 3) < 0)
// 	{
// 		perror("listen");
// 		exit(EXIT_FAILURE);
// 	}

// 	// Set the server socket to non-blocking
// 	fcntl(server_fd, F_SETFL, O_NONBLOCK);

// 	std::vector<struct pollfd> clients;

// 	struct pollfd server = {server_fd, POLLIN, 0};
// 	clients.push_back(server);


// 	std::cout << "server listening on port " << PORT << std::endl;
	
// 	while (true)
// 	{
// 		struct pollfd *fds = &clients.at(0);
// 		int ret = poll(fds, clients.size(), -1);

// 		if(ret < 0)
// 		{
// 			perror("poll");
// 			exit(EXIT_FAILURE);
// 		}

// 		// Check for an incoming connection
// 		if(clients.at(0).revents & POLLIN)
// 		{
// 			if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
// 			{
// 				perror("accept");
// 				exit(EXIT_FAILURE);
// 			}

// 			// Set the new socket to non-blocking
// 			fcntl(new_socket, F_SETFL, O_NONBLOCK);

// 			std::cout << "new client connected " << new_socket << std::endl;

// 			// Add the new socket to the array
// 			struct pollfd new_client = {new_socket, POLLIN | POLLOUT, 0};
// 			clients.push_back(new_client);
// 		}

// 		// Check each client for data
// 		for (unsigned long i = 1; i < clients.size(); i++)
// 		{
// 			struct pollfd client = clients.at(i);

// 			if(client.revents & POLLIN)
// 			{
// 				memset(buffer, 0, BUFFER_SIZE); // Clear the buffer
// 				ssize_t valread = recv(client.fd, buffer, BUFFER_SIZE - 1, 0);

// 				if(valread < 0)
// 				{
// 					// Handle errors (e.g., client disconnection)
// 				}
// 				else if(valread == 0)
// 				{
// 					// Handle client disconnection
// 					std::cout << "client " << client.fd << " disconnected" << std::endl;
// 					close(client.fd);
// 					clients.erase(clients.begin() + i);
// 					i--;
// 				}
// 				else
// 				{
// 					std::cout << "client " << client.fd << ": " << buffer;
// 				}
// 			}
// 			else if(client.revents & POLLOUT)
// 			{
// 				send(client.fd, "hello\r\n", 7, 0);
// 			}

// 		}
// 	}

// 	return 0;
// }
