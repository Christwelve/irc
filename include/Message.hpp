#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <string>
# include <vector>

class Message
{
	public:
		Message(const std::string &str);		// Constructor
		~Message();								// Destructor

		const std::string &getCommand() const;
		const std::string &getParamAt(unsigned long index) const;
		std::string getTrailing() const;
		bool hasTrailing() const;
		size_t getParamCount() const;

	private:

		std::string command_;
		std::vector<std::string> params_;
		std::string trailing_;

};

#endif
