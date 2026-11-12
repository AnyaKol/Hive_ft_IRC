# Colors
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
CYAN = \033[1;36m
RESET = \033[0m

NAME = ircserv

CXX ?= c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++20 -I./inc -I./inc/channelCommands
DEPFLAGS = -MMD -MP

SRC_DIR = src
CHAN_DIR = channelCommands

SRCS = main.cpp \
		Server.cpp \
		Parser.cpp \
		Client.cpp \
		CommandHandler.cpp \
		AuthCommands.cpp \
		Channel.cpp \
		AChannelCommand.cpp \
		Invite.cpp \
		Join.cpp \
		Kick.cpp \
		Mode.cpp \
		Names.cpp \
		Part.cpp \
		Topic.cpp

OBJ_DIR = obj

OBJS = $(SRCS:%.cpp=$(OBJ_DIR)/%.o)

DEPS = $(OBJS:%.o=%.d)

# ------------  VPATH  ------------------------------------------------------- #
VPATH = $(SRC_DIR)/: \
	$(SRC_DIR)/$(CHAN_DIR):

# ------------  RULES  ------------------------------------------------------- #
all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@
	@printf "$(GREEN)✓ $(NAME) built successfully!$(RESET)\n"
	@printf "  Usage: $(YELLOW)./$(NAME) <port> <password>$(RESET)\n"

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@
	@printf "$(CYAN)  Compiled $<$(RESET)\n"

clean:
	rm -rf $(OBJ_DIR)
	@printf "$(YELLOW)✓ Object files cleaned!$(RESET)\n"

fclean: clean
	rm -f $(NAME)
	@printf "$(RED)✓ Executable $(NAME) removed!$(RESET)\n"

re: fclean all

# ------------  EXTRA  ------------------------------------------------------- #
.PHONY: all clean fclean re
.SILENT:
.SECONDARY: $(OBJS), $(OBJ_DIR)
