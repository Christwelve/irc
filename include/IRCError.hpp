#ifndef IRCERROR_HPP
# define IRCERROR_HPP

# include <exception>
# include <string>
# include <cstring>

# define RED "\033[1;31m"
# define GREEN "\033[1;32m"
# define CLEAR "\033[0m"

class IRCError: public std::exception
{
	private:
		const std::string msg_;

	public:
		IRCError(const std::string &msg): msg_(RED + std::string("Error: ") + CLEAR + msg) {};

		~IRCError() throw() {};

		const char *what() const throw()
		{
			return (msg_.c_str());
		};
};

#endif
