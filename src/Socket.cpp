#include <fcntl.h>
#include <unistd.h>
#include "Socket.hpp"
#include "IRCError.hpp"

#include <iostream>
#include <arpa/inet.h>

Socket::Socket(void)
{
	fd_ = -1;
	isServer_ = false;
	memset(&pollfd_, 0, sizeof(pollfd_));
	memset(&address_, 0, sizeof(address_));
}

Socket::Socket(int port)
{
	fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (fd_ == -1)
		throw IRCError("Failed to create server socket");

	int opt = 1;
	if(setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw IRCError("Failed to set socket options");

	memset(&address_, 0, sizeof(address_));
	address_.sin_family = AF_INET;
	address_.sin_addr.s_addr = INADDR_ANY;
	address_.sin_port = htons(port);

	if(bind(fd_, (struct sockaddr *)&address_, sizeof(address_)) == -1)
		throw IRCError("Failed to bind socket to port");

	std::cout << "server running on ip " << inet_ntoa(address_.sin_addr) << std::endl;

	if(listen(fd_, SOMAXCONN) == -1)
		throw IRCError("Failed to listen on socket");

	fcntl(fd_, F_SETFL, O_NONBLOCK);

	pollfd_.fd = fd_;
	pollfd_.events = POLLIN;
	pollfd_.revents = 0;

	isServer_ = true;
}

Socket::Socket(int serverFd, bool isServer)
{
	(void) isServer;

	memset(&address_, 0, sizeof(address_));
    socklen_t addrlen = sizeof(address_);

	fd_ = accept(serverFd, (struct sockaddr *) &address_, &addrlen);
	if (fd_ == -1)
		throw IRCError("Failed to accept socket connection");

	fcntl(fd_, F_SETFL, O_NONBLOCK);

	pollfd_.fd = fd_;
	pollfd_.events = POLLIN | POLLOUT;
	pollfd_.revents = 0;

	isServer_ = false;
}

Socket::Socket(const Socket &socket)
{
	*this = socket;
}

Socket &Socket::operator=(const Socket &socket)
{
	if (this == &socket)
		return (*this);

	fd_ = socket.fd_;
	isServer_ = socket.isServer_;
	pollfd_ = socket.pollfd_;
	address_ = socket.address_;

	return (*this);
}

Socket::~Socket() {}

int Socket::getFd(void) const
{
	return (fd_);
}

struct sockaddr_in *Socket::getAddress(void) const
{
	return ((struct sockaddr_in *) &address_);
}

std::string Socket::getIp(void) const
{
	return (inet_ntoa(address_.sin_addr));
}

struct pollfd &Socket::getPollFd(void) const
{
	return ((struct pollfd &) pollfd_);
}

void Socket::setState(short state)
{
	pollfd_.revents = state;
}

bool Socket::hasPollIn(void) const
{
	return (!!(pollfd_.revents & POLLIN));
}

bool Socket::hasPollOut(void) const
{
	return (!!(pollfd_.revents & POLLOUT));
}

bool Socket::hasPollErr(void) const
{
	return (!!(pollfd_.revents & POLLERR));
}

void Socket::send(const std::string &msg)
{
	if(isServer_)
		throw IRCError("Sending to server socket is not permitted");

	::send(fd_, (msg + "\r\n").c_str(), msg.length() + 2, 0);
}

void Socket::close(void)
{
	::close(fd_);
}

bool Socket::operator==(const Socket &socket) const
{
	return (fd_ == socket.fd_);
}

bool Socket::operator!=(const Socket &socket) const
{
	return (fd_ != socket.fd_);
}
