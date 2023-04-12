<center>

# **FT_IRC - Internet Relay Chat**

[![sel-mars's 42 ft_irc Score](https://badge42.vercel.app/api/v2/cl49y2gyb004909l3hwkwo9dg/project/3014927)](https://github.com/JaeSeoKim/badge42)

</center>

---

## Table of Contents

-   [Summary](#summary)
-   [Introduction](#introduction)
-   [General Rules](#general-rules)
-   [Features](#features)
-   [Getting Started](#getting-started)
-   [Usage](#usage)
-   [Commands](#commands)
-   [Resources](#resources)
-   [Authors](#authors)
-   [Contributing](#contributing)
-   [License](#license)

### _Summary_

`FT_IRC` is an IRC server project that enables real-time messaging through text-based communication protocol on the Internet. The server is compliant with [RFC 2812](https://www.rfc-editor.org/rfc/rfc2812) and allows users to exchange direct messages and join group channels.

### _Introduction_

Internet Relay Chat (IRC) is a protocol that enables users to communicate with each other in real-time via text. It was created in the late 1980s and has been a popular form of communication on the Internet ever since. The protocol offers public and private messaging, as well as the ability to join channels with multiple users. Clients connect to servers in order to access the IRC network, and servers can be connected together to form a larger network.

### _General Rules_

The program is designed not to crash in any circumstance, including running out of memory or quitting unexpectedly. A Makefile is included that compiles the source files and includes rules for `$(NAME)`, `all`, `clean`, `fclean`, and `re`. The code is written in C++ 98 and complies with that standard. There are no external libraries or Boost libraries used.

### _Features_

`FT_IRC` server project comes with the following features:

-   Multiple clients can be handled simultaneously without ever hanging
-   Non-blocking I/O operations with only one poll() used to handle all operations (read, write, listen, etc.)
-   Supports authentication, setting a nickname, a username, joining a channel, and sending and receiving private messages
-   Messages sent from one client to a channel are forwarded to every other client that joined the channel
-   Supports operators and regular users
-   Implements commands that are specific to operators
-   Supports [LimeChat] as a reference client for evaluation purposes
-   File transfer using DCC in [LimeChat]
-   A bot using the [`42 API`](https://api.intra.42.fr/apidoc) to retrieve logtime of the current month, current location or coalition of any user

### _Getting Started_

Follow these simple steps to get started with the IRC Server Project:

1. Clone the repository to your local machine:

```sh
https://github.com/soofiane262/ft_irc.git
```

2. Navigate to the project directory:

```sh
cd ft_irc
```

3. Compile the source files by running the make command:

```sh
make
```

4. Start the IRC server:

```sh
./ircserv <port> <password>
```

Replace `<port>` with the port number on which the server will be listening for incoming IRC connections and `<password>` with the connection password.

5. Connect to the server using an IRC client such as [LimeChat] or by using netcat with the following command:

```sh
nc -c localhost <port>
```

6. Authenticate with the server password and set your nickname and username:

```sh
PASS <password>
NICK <nickname>
USER <username> <mode> * :<realname>
```

7. Join a channel and start sending messages to other users:

```sh
JOIN <channel>
PRIVMSG <channel> :Hello fellow peers :)
```

**Congratulations!** You're now connected to your own IRC server and can start chatting with others.

### _Usage_

After starting the server, clients can connect to it and communicate with each other through various IRC operations. They can join or create channels, send messages to other users or channels, and perform various IRC operations using the supported commands listed below.

### _Commands_

Our server supports several commands, conforming to the [BNF] representation specified in RFC2812. Here are the commands and their [BNF] representations:

```bnf
Command: AWAY
Parameters: [ <text> ]
```

The AWAY command is used to set an away message for the user. If the `<text>` parameter is included, it specifies the away message. If the `<text>` parameter is omitted, it means that the user is no longer away.

```bnf
Command: BOT
Parameters: <botcmd> [ <botparams> ]
```

The BOT command is used to send a command to a bot in the channel. The `<botcmd>` parameter specifies the command to be executed, and the `<botparams>` parameter provides any additional parameters that the command requires.

```bnf
Command: DIE
Parameters: None
```

The DIE command is used to shut down the server. This command is only available to server operators.

```bnf
Command: INVITE
Parameters: <nickname> <channel>
```

The INVITE command is used to invite a user to a specific channel. The `<nickname>` parameter specifies the user to be invited, and the `<channel>` parameter specifies the channel to which the user is to be invited.

```bnf
Command: JOIN
Parameters: <channel> [ <key> ]
```

The JOIN command is used to join a specific channel. The `<channel>` parameter specifies the channel to be joined, and the `<key>` parameter provides the channel password, if required.

```bnf
Command: KICK
Parameters: <channel> <user> [ <reason> ]
```

The KICK command is used to kick a user from a specific channel. The `<channel>` parameter specifies the channel from which the user is to be kicked, and the `<user>` parameter specifies the user to be kicked. The `<reason>` parameter provides the reason for the kick.

```bnf
Command: LIST
Parameters: [ <channels> [ <server> ] ]
```

The LIST command is used to list all channels on the server. The `<channels>` parameter specifies the channels to be listed, and the `<server>` parameter specifies the server to be queried. If the `<channels>` and `<server>` parameters are omitted, the server will list all channels on the server.

```bnf
Command: MODE
Parameters: <channel> <modes> [ <modeparams> ]
```

The MODE command is used to set or query the mode of a specific channel. The `<channel>` parameter specifies the channel to be modified, the `<modes>` parameter specifies the mode(s) to be set, and the `<modeparams>` parameter provides any additional parameters required for the mode(s).

```bnf
Command: NAMES
Parameters: [ <channels> [ <server> ] ]
```

The NAMES command is used to list all users in a specific channel or all channels on the server. The `<channels>` parameter specifies the channel(s) to be listed, and the `<server>` parameter specifies the server to be queried. If the `<channels>` and `<server>` parameters are omitted, the server will list all users in all channels on the server.

```bnf
Command: NICK
Parameters: <nickname>
```

The NICK command is used to change a user's nickname. The `<nickname>` parameter specifies the new nickname.

```bnf
Command: NOTICE
Parameters: <nickname> <text>
```

The NOTICE command is used to send a notice message to a specific user. The `<nickname>` parameter specifies the user to whom the notice is to be sent, and the `<text>` parameter specifies the text of the notice.

```bnf
Command: OPER
Parameters: <username> <password>
```

Oper command is used to authenticate a user as an IRC operator or "ircop" (one who manages an IRC network) with a password.

```bnf
Command: PART
Parameters: <channel> [ <reason> ]
```

The PART command is used to leave a specific channel. The `<channel>` parameter specifies the channel to be left, and the `<reason>` parameter provides the reason for leaving the channel.

```bnf
Command: PASS
Parameters: <password>
```

The PASS command is used to provide a password for the connection. The `<password>` parameter specifies the password to be used.

```bnf
Command: PONG
Parameters: <server>
```

The PONG command is used to respond to a PING command. The `<server>` parameter specifies the server that sent the PING command.
In our case, [LimeChat] automatically sends a PONG command after a specific amount of time.

```bnf
Command: PRIVMSG
Parameters: <receiver> <text>
```

The PRIVMSG command is used to send a private message to a specific user or channel. The `<receiver>` parameter specifies the user or channel to whom the message is to be sent, and the `<text>` parameter specifies the text of the message.

```bnf
Command: QUIT
Parameters: [ <reason> ]
```

The QUIT command is used to disconnect from the server. The `<reason>` parameter provides the reason for disconnecting from the server.

```bnf
Command: RESTART
Parameters: None
```

The RESTART command is used to restart the server. This command is only available to server operators.

```bnf
Command: TOPIC
Parameters: <channel> [ <topic> ]
```

The TOPIC command is used to set or retrieve the topic of a channel.

```bnf
Command: USER
Parameters: <username> <mode> <unused> <realname>
```

The USER command is used to provide information about the user. The `<username>` parameter specifies the username of the user, the `<mode>` parameter should be a numeric, and can be used to automatically set user modes when registering with the server. This parameter is a bitmask, with only 2 bits having any signification, and the `<realname>` parameter specifies the realname of the user.

```bnf
Command: WALLOPS
Parameters: <Text to be sent>
```

The WALLOPS command is used to send a message to all currently connected users who have set the 'w' user mode for themselves.

```bnf
Command: WHO
Parameters: <name> / "0"
```

The WHO command is used to list all users on the server. The `<name>` parameter specifies the name of the user to be listed, and the "0" parameter is a wildcard that is used to list all users on the server and who don't have a common channel with the requesting client.

```bnf
Command: WHOIS
Parameters: <nickname> [ <server> ]
```

The WHOIS command is used to query information about a specific user. The `<nickname>` parameter specifies the nickname of the user to be queried, and the `<server>` parameter specifies the server to be queried.

### _Resources_

-   The [RFC 2810 Document] defines the Internet Relay Chat (IRC) protocol describing the architecture of the current IRC protocol and the role of its different components.
-   The [RFC 2812 Document] defines the Internet Relay Chat (IRC) protocol and provides a detailed description of the messages exchanged between IRC clients and servers. Reading this document is essential to understand how the protocol works.
-   The [RFC 2811 Document] defines the Internet Relay Chat (IRC) protocol and provides a detailed description of how channels, their characteristics and properties are managed by IRC servers
-   The [Beej's Guide to Network Programming] is a comprehensive guide to network programming in C that covers sockets, networking protocols, and more. Although it's not specific to IRC server development, it can provide useful information about networking concepts and techniques.
-   The [IRCd-Hybrid project] is an open-source IRC server written in C. You can study its source code to get a better understanding of how an IRC server works and how to implement one in C++98.

### _Authors_

The `FT_IRC` server project was created by a team of developers from [1337], a coding school located in Morocco, Africa, that is a member of the renowned [42 Network] of coding schools. The school is also powered by the [Mohammed VI Polytechnic University (UM6P)](https://www.um6p.ma), which provides world-class resources and expertise to its students.

As a member of the [42 Network], [1337 Coding School] follows the same innovative and immersive educational model, which is based on project-based learning and peer-to-peer collaboration. This means that students are free to learn at their own pace, and are encouraged to work on real-world projects that challenge and inspire them.

The school's partnership with [UM6P] ensures that students have access to the latest technologies and tools, as well as world-class faculty members and experts in the field of computer science. This allows students to gain a comprehensive understanding of coding and computer science, while also developing the practical skills needed to succeed in the tech industry.

Overall, [1337 Coding School] is a unique and innovative institution that provides an exceptional education in coding and computer science, while also preparing students for success in the rapidly-evolving tech industry.

The project was completed as part of the common core's curriculum for the school's network branch.

The following individuals contributed to the development of the project:

| Name                             | Email                    |
| -------------------------------- | ------------------------ |
| [Achraf Maghous]                 | acmaghou@student.1337.ma |
| [Mohammed Badr Eddine El Housni] | mel-hous@student.1337.ma |
| [Soufiane El Marsi]              | sel-mars@student.1337.ma |

Each team member worked collaboratively to ensure the project met all the requirements, and together they successfully created a fully functional IRC server that meets the mandatory and bonus parts of the project.

### _Contributing_

Contributions to this project are welcome and encouraged. If you would like to contribute, please follow these steps:

1. Fork this repository.
2. Create a new branch with your changes: git checkout -b my-feature-branch
3. Commit your changes: git commit -am 'Add some feature'
4. Push the branch to your fork: git push origin my-feature-branch
5. Create a new Pull Request and describe your changes.

### _License_

This project is released under the permissive MIT License, which allows anyone to use, copy, modify, and distribute the software without any warranty. For more information about the license terms, please refer to the [LICENSE.md] file included in the project.

[limechat]: https://limechat.net/mac/
[bnf]: https://en.wikipedia.org/wiki/Backus%E2%80%93Naur_form
[rfc 2810 document]: https://www.rfc-editor.org/rfc/rfc2810
[rfc 2812 document]: https://www.rfc-editor.org/rfc/rfc2812
[rfc 2811 document]: https://www.rfc-editor.org/rfc/rfc2811
[beej's guide to network programming]: https://beej.us/guide/bgnet
[ircd-hybrid project]: http://www.ircd-hybrid.org
[42 network]: https://www.42network.org
[1337]: https://www.1337.ma/en
[1337 coding school]: https://www.1337.ma/en
[mohammed vi polytechnic university (um6p)]: https://www.um6p.ma
[um6p]: https://www.um6p.ma
[achraf maghous]: https://github.com/AchrafMaghous
[mohammed badr eddine el housni]: https://github.com/badr-7
[soufiane el marsi]: https://github.com/soofiane262
[license.md]: https://github.com/soofiane262/ft_irc/blob/master/LICENSE.md
