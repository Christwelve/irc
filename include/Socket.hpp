#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <sys/socket.h>
# include <netinet/in.h>
# include <poll.h>
# include <string>

class Socket
{
	private:

		int fd_;
		bool isServer_;
		struct pollfd pollfd_;
		struct sockaddr_in address_;

	public:
		Socket(void);
		Socket(int port);
		Socket(int serverFd, bool isClient);
		Socket(const Socket &socket);
		Socket &operator=(const Socket &socket);
		~Socket();

		int getFd(void) const;
		struct sockaddr_in *getAddress(void) const;
		std::string getIp(void) const;
		struct pollfd &getPollFd(void) const;
		void setState(short state);

		bool hasPollIn(void) const;
		bool hasPollOut(void) const;
		bool hasPollErr(void) const;

		void send(const std::string &msg);
		void close(void);

		bool operator==(const Socket &socket) const;
		bool operator!=(const Socket &socket) const;
};

#endif
