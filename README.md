# :eye: :registered: :ocean: 


/server add <server.name> <server.ip>/6667 -notls


# Commands

CAP 	->		ignore
QUIT	-> 		rm socket(fd), rm user from channels / delete channels, rm user from user.map, close socket
USER	->		addNewUser, User-, Real-, Nickname,  
JOIN	-> 		addUser to channel / create channel
PART	->		leave a channel
PRIVMSG	->		send message publicly or privatly 
KICK	->		(depending on own role) remove user from channel
INVITE	->		add user to channel
TOPIC	->		shows current topic, /topic <new_topic> replaces current topic
LIST	->		shows a list of all channels
MODE	->		shows current mode
	-i			set/unset invite only
	-t			set/unset /topic cmd
	-k			set/unset channel key
	-o			set/unset operator priviledge 


# Client -> Server communication

QUIT :<quit message>
NICK <new-nickname>
USER <username> <hostname> <servername> :<realname> 
JOIN <#channelname>
PART <#channelname>
PRIVMSG <#channelname>/<nickname> :<message>
PRIVMSG nickname :<message>
KICK <#channelname> <nickname> :<reason>
INVITE <nickname> <#channelname>
TOPIC <#channelname> :<new topic>
LIST
MODE <#channelname>/<nickname> <mode>


# Server -> Client communication

QUIT 	->		no response

NICK 	->		:NICK <old-nickname> <new-nickname>
				:server ERR_NICKNAMEINUSE * <new-nickname> :Nickname is already in use
				
USER 	->		:server 001 <nickname> :Welcome to the Internet Relay Network <nick>!<user>@<host>

JOIN 	->		:Nick!User@host JOIN :#channelname
				:server 332 <nickname> #channelname :<topic>
				:server 353 <nickname> = #channelname :<nicknames>

PART 	->		:Nick!User@host PART #channelname

PRIVMSG ->		:Nick!User@host PRIVMSG #channelname :<message> 
				:server ERR_CANNOTSENDTOCHAN <nickname> #channelname :Cannot send to channel

		->		:Nick!User@host PRIVMSG <nickname> :<message>
				:server ERR_NOSUCHNICK <nickname> :No such user

KICK 	->		:Nick!User@host KICK #channelname <nickname> :<reason>

INVITE 	->		:Nick!User@host INVITE <nickname> :#channelname

TOPIC 	->		:server 332 <nickname> #channelname :<topic>

LIST	->		:server 322 <nickname> #channelname <user count> :<topic>
				:server 323 <nickname> :End of LIST

MODE 	->		:Nick!User@host MODE #channelname :<mode>


# client 4: NICK cmeng
# client 4: USER cmeng 0 * :cmeng
USER <username> <hostname> <servername> :<realname>
In your example, client 4: USER cmeng 0 * :cmeng, the 0 and * are placeholders for the hostname and servername.
The 0 is a mode mask, often used to set user modes upon registration. In many IRC daemons, this field is ignored, but it's still part of the protocol. The 0 means no special modes are set.
The * is a wildcard. It's used here because the hostname and servername are typically provided by the server, not the client. The * indicates that any servername can be used.
So, in this case, the command is setting the username and realname to cmeng, with no specific hostname or servername.