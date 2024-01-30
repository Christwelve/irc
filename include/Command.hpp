#include <string>
#include "Parsing.hpp"
#include "User.hpp"
#include "Message.hpp"

enum Mode
{
	MODE_INVITE,
	MODE_TOPIC,
	MODE_KEY,
	MODE_OP,
	MODE_LIMIT,
	MODE_INVALID
};

enum ModeType
{
	MODE_TYPE_SET,
	MODE_TYPE_UNSET,
	MODE_TYPE_INVALID
};

class Command {
public:
    static std::string pass(User &user, const Message &message);
	static std::string ping(User &user, const Message &message);
	static std::string cap(User &user, const Message &message);
	static std::string quit(User &user, const Message &message);
	static std::string nick(User &user, const Message &message);
	static std::string user(User &user, const Message &message);
	static std::string join(User &user, const Message &message);
	static std::string part(User &user, const Message &message);
	static std::string privmsg(User &user, const Message &message);
	static std::string kick(User &user, const Message &message);
	static std::string invite(User &user, const Message &message);
	static std::string topic(User &user, const Message &message);
	static std::string mode(User &user, const Message &message);
};
