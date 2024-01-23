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

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define CLEAR "\033[0m"

# define BUFFER_SIZE 1024


class Server
{
	public:
		static Server &getInstance(void);

		~Server();						

		void initServer(size_t port, const std::string &password);
		void runServer(void);
		void shutdownServer(void);
		static void signalHandler(int signal);

	private:
		Server(void);	                        
		size_t port_;
		std::string password_;
		struct sockaddr_in address_;
		std::vector<struct pollfd> clients_;
		char buffer_[BUFFER_SIZE];
};

#endif
