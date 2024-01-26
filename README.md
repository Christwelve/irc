# :eye: :registered: :ocean: 


/server add <server.name> <server.ip>/6667 -notls


# Commands

CAP -> ignore

QUIT -> rm socket(fd), rm user from channels / delete channels, rm user from user.map, close socket

USER -> addNewUser, User-, Real-, Nickname,  

JOIN -> addUser to channel / create channel

PART ->	leave a channel

PRIVMSG -> send message publicly or privatly 

KICK -> (depending on own role) remove user from channel

INVITE -> add user to channel

TOPIC -> shows current topic, /topic <new_topic> replaces current topic

LIST -> shows a list of all channels

MODE -> shows current mode
	
 -i set/unset invite only

 -t set/unset /topic cmd

 -k set/unset channel key

 -o set/unset operator priviledge 


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

QUIT 	->		(to all other users of a channel): :Nickname!Username@hostname QUIT :Quit: quit message

NICK 	->		:NICK <old-nickname> <new-nickname>
			:server ERR_NICKNAMEINUSE * <new-nickname> :Nickname is already in use
				
USER 	->		:server 001 <nickname> :Welcome to the Internet Relay Network <nick>!<user>@<host>

JOIN 	->		:Nick!User@host JOIN :#channelname
			:server 332 <nickname> #channelname :<topic>
			:server 353 <nickname> = #channelname :<nicknames>

PART 	->		:Nick!User@host PART #channelname

PRIVMSG ->		:Nick!User@host PRIVMSG #channelname :<message> 
			:server ERR_CANNOTSENDTOCHAN <nickname> #channelname :Cannot send to channel

PRIVMSG	->		:Nick!User@host PRIVMSG <nickname> :<message>
			:server ERR_NOSUCHNICK <nickname> :No such user

KICK 	->		:Nick!User@host KICK #channelname <nickname> :<reason>

INVITE 	->		:Nick!User@host INVITE <nickname> :#channelname

TOPIC 	->		:server 332 <nickname> #channelname :<topic>

LIST	->		:server 322 <nickname> #channelname <user count> :<topic>
			:server 323 <nickname> :End of LIST

MODE 	->		:Nick!User@host MODE #channelname :<mode>




:server 421 client :Unknown command
001: Welcome message sent after a successful connection.
002: Server's version information.
003: Server's creation date.
004: Server's details (name, version, available user modes, available channel modes).
005: Additional server features.
353: Reply to the NAMES command, listing all visible users in a channel.
366: End of NAMES list.
372: MOTD (Message of the Day) text line.
375: Start of MOTD.
376: End of MOTD.
421: Unknown command.
433: Nickname is already in use.
441: User not in channel (used when trying to kick a user not in channel).
442: Not on channel (used when trying to perform a channel operation without being on the channel).
461: Need more parameters (used when a command needs more parameters).
462: Already registered (used when a client tries to register again).
464: Incorrect password.
465: You are banned from this server.
471: Channel is full.
473: Invite only channel.
474: Banned from channel.
475: Bad channel key.


>client 4: NICK cmeng
>
>client 4: USER cmeng 0 * :cmeng
>
>USER <username> <hostname> <servername> :<realname>
>
>In your example, client 4: USER cmeng 0 * :cmeng, the 0 and * are placeholders for the hostname and servername.
The 0 is a mode mask, often used to set user modes upon registration. In many IRC daemons, this field is ignored, but it's still part of the protocol. The 0 means no special modes are set.
The * is a wildcard. It's used here because the hostname and servername are typically provided by the server, not the client. The * indicates that any servername can be used.
So, in this case, the command is setting the username and realname to cmeng, with no specific hostname or servername.
