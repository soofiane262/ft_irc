# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/08 14:44:10 by sel-mars          #+#    #+#              #
#    Updated: 2023/03/31 17:08:21 by sel-mars         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv

CPP		=	c++

FLAGS	=	-Wall -Wextra -Werror -std=c++98

COMMANDS = $(addprefix commands/,$(addsuffix .cpp,\
			commands JOIN NICK PASS PONG QUIT NOTICE USER PRIVMSG PART MODE\
			TOPIC LIST NAMES KICK WHO OPER INVITE WHOIS AWAY))

SERVER = $(addprefix server/,$(addsuffix .cpp,\
			channel_ops client_ops message_ops server_ops server))

SRCS	=	main.cpp  $(addprefix srcs/,\
			message.cpp utils.cpp client.cpp channel.cpp $(COMMANDS) $(SERVER))

all: $(NAME)

$(NAME): $(SRCS)
	@$(CPP) $(FLAGS) $(SRCS) -o $@

clean:
	@rm -f $(NAME)

fclean: clean

re: fclean all

.PHONY: all $(NAME) clean fclean re
