# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acmaghou <acmaghou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/08 14:44:10 by sel-mars          #+#    #+#              #
#    Updated: 2023/04/02 01:08:46 by acmaghou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ircserv

CPP			= c++

FLAGS		= -Wall -Wextra -Werror -std=c++98 -O3 -MMD -MP

COMMANDS	= $(addprefix commands/,$(addsuffix .cpp,\
				commands JOIN NICK PASS PONG QUIT NOTICE USER PRIVMSG PART MODE\
				TOPIC LIST NAMES KICK WHO OPER INVITE WHOIS AWAY BOT))

SERVER		= $(addprefix server/,$(addsuffix .cpp,\
				channel_ops client_ops message_ops server_ops server))

SRCS		= main.cpp $(addprefix srcs/,\
				message.cpp utils.cpp client.cpp channel.cpp $(COMMANDS) $(SERVER))

HEADERS		= $(addprefix srcs/,irc.hpp definitions.hpp)

OBJS		= $(SRCS:%.cpp=obj/%.o)

DEPS		= $(OBJS:.o=.d)

obj/%.o: %.cpp $(HEADERS)
	@mkdir -p $(@D)
	@$(CPP) $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CPP) $(FLAGS) $(OBJS) -o $(NAME)

all: $(NAME)

clean:
	@rm -rf obj

fclean: clean
	@rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
