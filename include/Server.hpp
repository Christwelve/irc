#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <sys/socket.h>
# include <netinet/in.h>
# include <fcntl.h>
# include <unistd.h>
# include <poll.h>
# include <vector>
# include <signal.h>
# include <cstddef>
# include "Socket.hpp"

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define CLEAR "\033[0m"

# define BUFFER_SIZE 1024


class Server
{
	public:
		static Server &getInstance(void);

		~Server();

		void initServer(int port, const std::string &password);
		void runServer(void);
		void shutdownServer(void);
		static void signalHandler(int signal);

	private:
		Server(void);
		std::string password_;
		Socket socket_;
		std::vector<Socket> clients_;
		bool running_;

		void pollSockets(void);
		void listenForNewClients(void);
		void processClientSockets(void);

		bool isPasswordValid(const std::string &password) const;
};

#endif
