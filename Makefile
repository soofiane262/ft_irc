# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/08 14:44:10 by sel-mars          #+#    #+#              #
#    Updated: 2023/03/20 17:10:21 by sel-mars         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv

CPP		=	c++

FLAGS	=	-O3 -std=c++98 -Wall -Wextra -Werror -Wpedantic -Wno-c++11-extensions

COMMANDS = $(addprefix commands/,commands.cpp JOIN.cpp NICK.cpp PASS.cpp PONG.cpp QUIT.cpp USER.cpp)

SERVER = $(addprefix server/, channel_ops.cpp client_ops.cpp message_ops.cpp server_ops.cpp server.cpp)

CHANNEL = $(addprefix channel/, channel.cpp)

SRCS	=	main.cpp  $(addprefix srcs/,message.cpp utils.cpp $(COMMANDS) $(SERVER) $(CHANNEL))

all: $(NAME)

$(NAME): $(SRCS)
	@$(CPP) $(FLAGS) $(SRCS) -o $@

clean:
	@rm -f $(NAME)

fclean: clean

re: fclean all

.PHONY: all $(NAME) clean fclean re
