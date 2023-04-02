# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sel-mars <sel-mars@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/08 14:44:10 by sel-mars          #+#    #+#              #
#    Updated: 2023/04/01 21:17:16 by sel-mars         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Define the project name and compiler
NAME			:= ircserv
CPP				:= c++

# Compiler flags
FLAGS			:= -Wall -Wextra -Werror -std=c++98 -O3 -MMD -MP

# Define the source files and directories
COMMANDS_DIR	:= commands
ASSETS_DIR		:= assets

# List of command and asset files to compile
COMMANDS		:= $(addprefix $(COMMANDS_DIR)/,$(addsuffix .cpp, \
	AWAY BOT INVITE JOIN KICK LIST MODE NAMES NICK NOTICE \
	OPER PART PASS PONG PRIVMSG QUIT TOPIC USER WHO WHOIS))
ASSETS			:= $(addprefix $(ASSETS_DIR)/, $(addsuffix .cpp, \
	channel client commands message server utils))
SRCS			:= irc.cpp $(COMMANDS) $(ASSETS)

# Define the header files
HEADERS			:= irc.hpp assets/definitions.hpp

# Define the object files and dependencies
OBJS			:= $(patsubst %.cpp, obj/%.o, $(SRCS))
DEPS			:= $(OBJS:.o=.d)

# Compile each source file into its corresponding object file
obj/%.o: %.cpp $(HEADERS)
	@mkdir -p $(@D)
	@$(CPP) $(FLAGS) -c $< -o $@

# Link the object files into the executable
$(NAME): $(OBJS)
	@$(CPP) $(FLAGS) $(OBJS) -o $(NAME)

# Default target is to build the executable
all: $(NAME)

# Remove all generated object files
clean:
	@rm -rf obj

# Remove the executable and all generated object files
fclean: clean
	@rm -f $(NAME)

# Clean and rebuild the project
re: fclean all

# Include the generated dependencies
-include $(DEPS)

# Declare phony targets
.PHONY: all clean fclean re
