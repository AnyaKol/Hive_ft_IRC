# Hive_ft_IRC

Rank-05

This is a Hive school group project about creating IRC server for irssi client.


## To start the project

Open in terminal folder with the project and run:

> make
> ./ircserv <port> <password>

In another terminal window start **irssi client**:

> irssi
> /connect localhost <port> <password>


## Supported commands

User commands:

| COMMAND | <parameters> | Usage |
|:-------:|:-------------|:------|
| NICK | \<nickname\> | set user nickname |
| PASS | \<password\> | provide password |
| USER | \<username\> \<mode\> \<unused\> \<realname\> | show user info |
| PING | \<token\> | ping |
| PONG | \<token\> | pong |
| QUIT | \[message\] | exit irssi |
| PRIVMSG | \<target\> \<message\> | send private message |



Channel commands:

| COMMAND | <parameters> | Usage |
|:-------:|:-------------|:------|
| INVITE | \<nickname\> \<channel\> | invite user to channel |
| JOIN | \<channel\>\{,\<channel\>\} \[\<key\>\{,\<key\>\}\] | join channel |
| KICK | \<channel\> \<user\> *( "," \<user\> ) \[\<comment\>\] | remove user from channel |
| MODE | \<target\> \[\<modestring\> \[\<mode arguments\>...\]\] | change channel modes |
| NAMES | \<channel\>\{"," \<channel\>\} | show channel members |
| PART | \<channel\>\{"," \<channel\>\} \[\<reason\>\] | leave channel |
| TOPIC | \<channel\> \[\<topic\>\] | change channel topic |



Channel modes:

- i - make channel *invite only*;
- t - make *topic protected*;
- k - set/remove channel *password*;
- l - set/remove *user limit*;
- o - add/remove user *operator priviliges*.
