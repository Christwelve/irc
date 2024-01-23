
#ifndef IRCERROR_HPP
# define IRCERROR_HPP

#include <exception>
#include <string>
#include <cstring>

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define CLEAR "\033[0m"

class IRCError: public std::exception
{
    protected:
        char *errorMessage_;

    public:
        IRCError(const std::string &msg) {
            std::string errorMessage = RED + std::string("Error: ") + CLEAR + msg;
            errorMessage_ = new char[errorMessage.length() + 1];
            strcpy(errorMessage_, errorMessage.c_str());
        };

        ~IRCError() throw()
        {
            delete errorMessage_;
        };

        const char *what() const throw();
};


#endif 