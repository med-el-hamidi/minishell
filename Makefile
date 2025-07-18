# ============================================================================ #
#                               MINISHELL MAKEFILE                             #
# ============================================================================ #
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
CFLAGS		:= -Wall -Wextra -Werror -I$(HOME)/brew/opt/readline/include  -fsanitize=address
LDFLAGS		:= -Llibft -lft -L$(HOME)/brew/opt/readline/lib -lreadline
LIBFT		:= libft/libft.a

# Header files
INCLUDES	:=	libft/libft.h \
				includes/minishell.h \
				includes/structures.h \
				includes/lexer.h \
				includes/exec.h

# Source directories
SRC_DIR		:= src
LEX_DIR		:= $(SRC_DIR)/lexer
PAR_DIR		:= $(SRC_DIR)/parser
EXE_DIR		:= $(SRC_DIR)/executor
BLT_DIR		:= $(EXE_DIR)/builtins
EXECVE_DIR	:= $(EXE_DIR)/execve_ext
LOC_VA_DIR	:= $(EXE_DIR)/local_vars
RED_PIP_DIR	:= $(EXE_DIR)/redirection_pipe
UTL_DIR		:= $(SRC_DIR)/utils

# Source files
SRCS		:= $(SRC_DIR)/main.c \
			$(UTL_DIR)/utils.c \
			$(UTL_DIR)/herdoc_utils.c \
			$(UTL_DIR)/init_utils.c \
			$(UTL_DIR)/init.c \
			$(UTL_DIR)/signal.c \
			$(UTL_DIR)/error.c \
			$(UTL_DIR)/free.c \
			$(UTL_DIR)/histfile_utils.c \
			$(UTL_DIR)/history_utils.c \
			$(UTL_DIR)/history.c \
			$(LEX_DIR)/ft_split_set.c \
			$(LEX_DIR)/lexer_word_utils_0.c \
			$(LEX_DIR)/lexer_word_utils_1.c \
			$(LEX_DIR)/lexer_word.c \
			$(LEX_DIR)/lexer_herdoc_utils.c \
			$(LEX_DIR)/lexer_redir_utils.c \
			$(LEX_DIR)/lexer_redir.c \
			$(LEX_DIR)/lexer_utils.c \
			$(LEX_DIR)/lexer.c \
			$(PAR_DIR)/parse.c \
			$(PAR_DIR)/parse_redir.c \
			$(PAR_DIR)/parser_utils.c \
			$(PAR_DIR)/parser.c \
			$(BLT_DIR)/cd.c \
			$(BLT_DIR)/echo.c \
			$(BLT_DIR)/env.c \
			$(BLT_DIR)/exit.c \
			$(BLT_DIR)/pwd.c \
			$(BLT_DIR)/unset.c \
			$(BLT_DIR)/export.c \
			$(BLT_DIR)/export_utils_0.c \
			$(BLT_DIR)/export_utils_1.c \
			$(EXE_DIR)/is_builtin.c \
			$(EXE_DIR)/errors_utils.c \
			$(EXE_DIR)/executor.c \
			$(EXE_DIR)/utils.c \
			$(LOC_VA_DIR)/local_utils.c \
			$(LOC_VA_DIR)/exec_local_vars.c \
			$(EXECVE_DIR)/execve_utils.c \
			$(EXECVE_DIR)/execve.c \
			$(RED_PIP_DIR)/pipe.c \
			$(RED_PIP_DIR)/exec_redir.c \


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
