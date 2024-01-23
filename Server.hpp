#ifndef SERVER_HPP
# define SERVER_HPP

#define BUFFER_SIZE 1024


class Server
{
    public:
        Server(size_t port, char *password);	// Default Constructor
        ~Server();								// Destructor

        initServer(size_t port, char *password);
        runServer();
        shutdownServer(int server_fd, std::vector<struct pollfd> clients);

    private:
        int server_fd_;
        struct sockaddr_in address_;
        std::vector<struct pollfd> clients_;
        char *password_;
        chat buffer_[BUFFER_SIZE];


};

#endif