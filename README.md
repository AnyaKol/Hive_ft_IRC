# Hive_ft_IRC

Rank-05

This is a Hive school group project about creating IRC server for irssi client.


## To start the project

To start the server open in terminal folder with the project and run:

> make

> .\/ircserv \<port\> \<password\>


To connect with **irssi client** in another terminal window run:

> irssi

> \/connect localhost \<port\> \<password\>


To connect with netcat in another terminal window run:

> nc -C localhost \<port\>

> pass \<password\>

> nick \<nickname\>

> user \<username\> 0 * : \<real_name\> 


## Supported commands

\[ ... \] - optional parameters;
\{"," ... \} - allows multiple comma-separated arguments (no spaces).

User commands:

| Command | Parameters | Usage |
|:-------:|:-------------|:------|
| NICK | nickname | set user nickname |
| USER | username mode unused realname | show user info |
| QUIT | \[message\] | exit irssi |
| PRIVMSG | target message | send private message |


Channel commands:

| Command | Parameters | Usage |
|:-------:|:-------------|:------|
| INVITE | nickname channel | invite user to channel |
| JOIN | channel\{"," channel\} \[key\{"," key\}\] | join channel |
| KICK | channel user\{"," user\} \[comment\] | remove user from channel |
| MODE | target \[modestring \[mode arguments\]\] | change channel modes |
| NAMES | channel | show channel members |
| PART | channel\{"," channel\} \[reason\] | leave channel |
| TOPIC | channel \[topic\] | change channel topic |


Channel modes:

- i - make channel *invite only*;
- t - make *topic protected*;
- k - set/remove channel *password*;
- l - set/remove *user limit*;
- o - add/remove user *operator priviliges*.
