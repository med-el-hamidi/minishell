# ============================================================================ #
#                               MINISHELL MAKEFILE                             #
# ============================================================================ #
#                Conflict Zone
#                git pull -> update -> git push
#
# ******************************** COLORS ************************************ #
RED		:= \033[31m
GREEN	:= \033[32m
YELLOW	:= \033[33m
BLUE	:= \033[34m
BOLD	:= \033[1m
RESET	:= \033[0m

# ******************************* VARIABLES ********************************** #
NAME		:= minishell
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -fsanitize=address
LDFLAGS		:= -Llibft -lft
LIBFT		:= libft/libft.a

# Header files
INCLUDES	:=	libft/libft.h \
				includes/minishell.h \
				includes/structures.h

# Source directories
SRC_DIR		:= src
LEX_DIR		:= $(SRC_DIR)/lexer
PAR_DIR		:= $(SRC_DIR)/parser
EXE_DIR		:= $(SRC_DIR)/executor
BLT_DIR		:= $(SRC_DIR)/builtins

# Source files
SRCS		:= $(SRC_DIR)/main.c \

# Object files
OBJS		:= $(SRCS:.c=.o)

# ******************************** RULES ************************************* #

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@printf "$(GREEN)$(BOLD)✅ Minishell ready!$(RESET)\n"

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(BLUE)🔧 Building: $(notdir $<)$(RESET)\n"

$(LIBFT):
	make -C libft

all: $(NAME)

clean:
	make -C libft clean
	rm -f $(OBJS)
	@printf "$(YELLOW)🧹 Cleaned object files$(RESET)\n"

fclean: clean
	make -C libft fclean
	rm -f $(NAME)
	@printf "$(RED)🔥 Full clean complete$(RESET)\n"

re: fclean all

norm:
	@norminette $(SRC_DIR) includes
#   libft already tested :)
	@printf "$(YELLOW)🧐 Norm check completed$(RESET)\n"

.PHONY: clean norm
