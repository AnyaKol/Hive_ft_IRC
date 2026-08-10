# Colors
RED = \033[1;31m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
CYAN = \033[1;36m
RESET = \033[0m

NAME = ircserv

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -std=c++20
DEPFLAGS = -MMD -MP

SRCS = main.cpp

DEPS = $(patsubst %.o, %.d, $(OBJS))

OBJ_DIR = obj/

OBJS = $(addprefix $(OBJ_DIR), $(SRCS:%.cpp=%.o))

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@
	@printf "$(GREEN)✓ $(NAME) built successfully!$(RESET)\n"

$(OBJ_DIR)%.o: %.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@
	@printf "$(CYAN)  Compiled $<$(RESET)\n"

clean:
	rm -rf $(OBJ_DIR)
	@printf "$(YELLOW)✓ Object files cleaned!$(RESET)\n"

fclean: clean
	rm -f $(NAME)
	@printf "$(RED)✓ Executable $(NAME) removed!$(RESET)\n"

re: fclean all

-include $(DEPS)

.PHONY:	all clean fclean re

.SILENT:

.SECONDARY:
