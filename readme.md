# **FT_IRC - Internet Relay Chat**

## Table of Contents

-   [Summary](#summary)
-   [Introduction](#introduction)
-   [General Rules](#general-rules)
-   [Features](#features)
-   [Requirements](#requirements)
-   [Getting Started](#getting-started)
-   [Mandatory Part](#mandatory-part)
-   [Bonus Part](#bonus-part)
-   [Usage](#usage)
-   [Commands](#commands)
-   [Authors](#authors)
-   [Contributing](#contributing)

<!-- -	[License](#license) -->
<!-- -	[Contact](#contact) -->

### _Summary_

FT_IRC is an IRC server project that allows real-time messaging through text-based communication protocol on the Internet. It offers users the ability to exchange direct messages and join group channels. IRC clients connect to IRC servers to join channels, and IRC servers are connected together to form a network. Our server is compliant with [RFC 2812](https://www.rfc-editor.org/rfc/rfc2812). The Internet is ruled by solid standards protocols that allow connected computers to interact with each other. It’s always a good thing to know.

### _Introduction_

Internet Relay Chat (IRC) is a protocol that enables users to communicate with each other in real-time via text. It was created in the late 1980s and has been a popular form of communication on the Internet ever since. The protocol offers public and private messaging, as well as the ability to join channels with multiple users. Clients connect to servers in order to access the IRC network, and servers can be connected together to form a larger network.

### _General Rules_

Our program is designed to not crash in any circumstance, including running out of memory or quitting unexpectedly. We have included a Makefile that compiles the source files and includes rules for `$(NAME)`, `all`, `clean`, `fclean`, and `re`.

Our code is written in C++ 98 and complies with that standard. We have not used any external libraries or Boost libraries.

### _Features_

Our IRC server project comes with the following features:

-   Handles multiple clients simultaneously without ever hanging
-   Non-blocking I/O operations with only one poll() used to handle all operations (read, write, listen, etc.)
-   Supports authentication, setting a nickname, a username, joining a channel, and sending and receiving private messages
-   Messages sent from one client to a channel are forwarded to every other client that joined the channel
-   Supports operators and regular users
-   Implements commands that are specific to operators
-   File transfer using DCC in LimeChat
-   A bot using the [`42 API`](https://api.intra.42.fr/apidoc) to retrieve logtime of the current month, current location or coalition of any user

### _Mandatory Part_

Our IRC server is written in C++ 98 and can handle multiple clients simultaneously. It never hangs, and all I/O operations are non-blocking. We do not fork, and only one poll() function is used to handle all operations, including read, write, and listen.

Our server requires a password for connection, and clients must authenticate, set a nickname and username, join a channel, and send and receive private messages. All messages sent from one client to a channel are forwarded to every other client that joined the channel. The server has operators and regular users, and we have implemented commands specific to operators.

We have tested our server with the LimeChat IRC client, which can connect to our server without encountering any errors. Communication between the client and server is done via TCP/IP v4.

### _Bonus Part_

In addition to the mandatory requirements, we have added the ability to handle file transfer using DCC in LimeChat. We have also added a bot that uses the [`42 API`](https://api.intra.42.fr/apidoc) to retrieve logtime of the current month, current location, or coalition of any user.

### _Requirements_

Our IRC server project meets the following requirements:

-   Does not crash in any circumstances (even when it runs out of memory) and does not quit unexpectedly
-   Compiled using the provided Makefile which contains the rules: $(NAME), all, clean, fclean, and re
-   Complies with the C++ 98 standard and compiled with the flag -std=c++98
-   Does not use any external libraries or Boost libraries
-   Uses TCP/IP v4 for communication between client and server
-   Supports LimeChat as a reference client for evaluation purposes
-   Does not fork and uses non-blocking I/O operations
-   Uses only one poll() to handle all I/O operations

### _Getting Started_

To use our IRC server project, follow these steps:

1. Clone the repository to your local machine.

```sh
https://github.com/soofiane262/ft_irc.git
```

2. Navigate to the project directory.

```sh
cd ft_irc
```

3. Run make to compile the source files.

```sh
make
```

4. Run ./ircserv <port> <password> to start the IRC server, replacing <port> with the port number on which the server will be listening for incoming IRC connections and <password> with the connection password.

```sh
./ircserv <port> <password>
```

5. Connect to the server using an IRC client, such as LimeChat or using netcat.

```sh
nc -c localhost <port>
```

6. Authenticate with the server password, set a nickname and username.

```sh
PASS <password>
NICK <nickname>
USER <username> <mode> * :<realname>
```

7. Join a channel, and start sending messages to other users.

```sh
JOIN <channel>
PRIVMSG <channel> :Hello fellow peers :)
```

**That's it!** You're now connected to your own IRC server and can start chatting with others.

### _Usage_

Once the server is up and running, clients can connect to it and start chatting with each other. Clients can join or create channels, send messages to other users or channels, and perform various IRC operations using the supported commands listed below.

### _Commands_

Our server supports the following commands:

```sh
AWAY = "AWAY" [ <text> ]
```

The AWAY command is used to set an away message for the user. If the <text> parameter is included, it specifies the away message. If the <text> parameter is omitted, it means that the user is no longer away.

```sh
BOT = "BOT" <botcmd> [ <botparams> ]
```

The BOT command is used to send a command to a bot in the channel. The <botcmd> parameter specifies the command to be executed, and the <botparams> parameter provides any additional parameters that the command requires.

```sh
INVITE = "INVITE" <nickname> <channel>
```

The INVITE command is used to invite a user to a specific channel. The <nickname> parameter specifies the user to be invited, and the <channel> parameter specifies the channel to which the user is to be invited.

```sh
JOIN = "JOIN" <channel> [ <key> ]
```

The JOIN command is used to join a specific channel. The <channel> parameter specifies the channel to be joined, and the <key> parameter provides the channel password, if required.

```sh
KICK = "KICK" <channel> <user> [ <reason> ]
```

The KICK command is used to kick a user from a specific channel. The <channel> parameter specifies the channel from which the user is to be kicked, and the <user> parameter specifies the user to be kicked. The <reason> parameter provides the reason for the kick.

```sh
LIST = "LIST" [ <channels> [ <server> ] ]
```

The LIST command is used to list all channels on the server. The <channels> parameter specifies the channels to be listed, and the <server> parameter specifies the server to be queried. If the <channels> and <server> parameters are omitted, the server will list all channels on the server.

```sh
MODE = "MODE" <channel> <modes> [ <modeparams> ]
```

The MODE command is used to set or query the mode of a specific channel. The <channel> parameter specifies the channel to be modified, the <modes> parameter specifies the mode(s) to be set, and the <modeparams> parameter provides any additional parameters required for the mode(s).

```sh
NAMES = "NAMES" [ <channels> [ <server> ] ]
```

The NAMES command is used to list all users in a specific channel or all channels on the server. The <channels> parameter specifies the channel(s) to be listed, and the <server> parameter specifies the server to be queried. If the <channels> and <server> parameters are omitted, the server will list all users in all channels on the server.

```sh
NICK = "NICK" <nickname>
```

The NICK command is used to change a user's nickname. The <nickname> parameter specifies the new nickname.

```sh
NOTICE = "NOTICE" <nickname> <text>
```

The NOTICE command is used to send a notice message to a specific user. The <nickname> parameter specifies the user to whom the notice is to be sent, and the <text> parameter specifies the text of the notice.

```sh
OPER = "OPER" <username> <password>
```

Oper command is used to authenticate a user as an IRC operator or "ircop" (one who manages an IRC network) with a password.

```sh
PART = "PART" <channel> [ <reason> ]
```

The PART command is used to leave a specific channel. The <channel> parameter specifies the channel to be left, and the <reason> parameter provides the reason for leaving the channel.

```sh
PASS = "PASS" <password>
```

The PASS command is used to provide a password for the connection. The <password> parameter specifies the password to be used.

```sh
PONG = "PONG" <server1> [ <server2> ]
```

The PONG command is used to respond to a PING command. The <server1> parameter specifies the server that sent the PING command, and the <server2> parameter specifies the server that received the PING command.

```sh
PRIVMSG = "PRIVMSG" <receiver> <text>
```

The PRIVMSG command is used to send a private message to a specific user or channel. The <receiver> parameter specifies the user or channel to whom the message is to be sent, and the <text> parameter specifies the text of the message.

```sh
QUIT = "QUIT" [ <reason> ]
```

The QUIT command is used to disconnect from the server. The <reason> parameter provides the reason for disconnecting from the server.

```sh
TOPIC = "TOPIC" <channel> [ <topic> ]
```

The TOPIC command is used to set or retrieve the topic of a channel.

```sh
USER = "USER" <username> <hostname> <servername> <realname>
```

The USER command is used to provide information about the user. The <username> parameter specifies the username of the user, the <hostname> parameter specifies the hostname of the user, the <servername> parameter specifies the servername of the user, and the <realname> parameter specifies the realname of the user.

```sh
WHO = "WHO" [ <name> [ <o> ] ]
```

The WHO command is used to list all users on the server. The <name> parameter specifies the name of the user to be listed, and the <o> parameter specifies whether to list only operators.

```sh
WHOIS = "WHOIS" <nickname> [ <server> ]
```

The WHOIS command is used to query information about a specific user. The <nickname> parameter specifies the nickname of the user to be queried, and the <server> parameter specifies the server to be queried.

### _Authors_

| Name                           | Email                    |
| ------------------------------ | ------------------------ |
| Achraf Maghous                 | acmaghou@student.1337.ma |
| Mohammed Badr Eddine El Housni | mel-hous@student.1337.ma |
| Soufiane El Marsi              | sel-mars@student.1337.ma |

### _Contributing_

Contributions to this project are welcome and encouraged. If you would like to contribute, please follow these steps:

1. Fork this repository.
2. Create a new branch with your changes: git checkout -b my-feature-branch
3. Commit your changes: git commit -am 'Add some feature'
4. Push the branch to your fork: git push origin my-feature-branch
5. Create a new Pull Request and describe your changes.

<!-- ### _License_

This project is licensed under the [MIT License](https://opensource.org/license/mit/). See the `LICENSE` file for more information.

### _Contact_

If you have any questions or suggestions, please feel free to contact the contributors of this project. We are always happy to hear from you! -->
