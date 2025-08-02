# ============================================================================ #
#                               MINISHELL MAKEFILE                             #
# ============================================================================ #

# ******************************* VARIABLES ********************************** #
NAME		:= minishell
BONUS_NAME	:= minishell_bonus
CC			:= cc
CFLAGS		:= -Wall -Wextra -Werror -I$(HOME)/brew/opt/readline/include
LDFLAGS		:= -Llibft -lft -L$(HOME)/brew/opt/readline/lib -lreadline
LIBFT		:= libft/libft.a
LIBFT_SRCS	:= libft/ft_isalpha.c libft/ft_isdigit.c libft/ft_isalnum.c libft/ft_isascii.c libft/ft_isprint.c libft/ft_strlen.c \
		libft/ft_bzero.c libft/ft_memcpy.c libft/ft_memmove.c libft/ft_strlcpy.c libft/ft_strlcat.c libft/ft_toupper.c \
		libft/ft_tolower.c libft/ft_strchr.c libft/ft_strrchr.c libft/ft_strncmp.c libft/ft_memcmp.c libft/ft_memset.c \
		libft/ft_strnstr.c libft/ft_atoi.c libft/ft_strdup.c libft/ft_calloc.c libft/ft_putchar_fd.c libft/ft_putstr_fd.c \
		libft/ft_substr.c libft/ft_strjoin.c libft/ft_strtrim.c libft/ft_strmapi.c libft/ft_putnbr_fd.c libft/ft_putendl_fd.c \
		libft/ft_striteri.c libft/ft_memchr.c libft/ft_split.c libft/ft_itoa.c \
		libft/ft_lstnew.c libft/ft_lstadd_front.c libft/ft_lstsize.c libft/ft_lstlast.c \
		libft/ft_lstadd_back.c libft/ft_lstdelone.c libft/ft_lstclear.c \
		libft/get_next_line.c libft/get_next_line_utils.c

# Header files
INCLUDES	:=	includes/minishell.h \
				includes/structures.h \
				includes/lexer.h \
				includes/parser.h \
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
			$(LEX_DIR)/lexer_redir_filename.c \
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
			$(EXE_DIR)/error.c \
			$(EXE_DIR)/executor.c \
			$(EXE_DIR)/utils.c \
			$(LOC_VA_DIR)/local_utils.c \
			$(LOC_VA_DIR)/exec_local_vars.c \
			$(EXECVE_DIR)/execve_utils.c \
			$(EXECVE_DIR)/execve.c \
			$(RED_PIP_DIR)/pipe.c \
			$(RED_PIP_DIR)/redirection.c

# ******************************* BONUS ********************************** #
BONUS_PATH := minishell_bonus_part

# BONUS Header files
BONUS_INCLUDES	:=	$(BONUS_PATH)/includes_bonus/minishell_bonus.h \
				$(BONUS_PATH)/includes_bonus/structures_bonus.h \
				$(BONUS_PATH)/includes_bonus/lexer_bonus.h \
				$(BONUS_PATH)/includes_bonus/parser_bonus.h \
				$(BONUS_PATH)/includes_bonus/exec_bonus.h

# BONUS Source directories
BONUS_SRC_DIR		:= $(BONUS_PATH)/src_bonus
BONUS_LEX_DIR		:= $(BONUS_SRC_DIR)/lexer_bonus
BONUS_PAR_DIR		:= $(BONUS_SRC_DIR)/parser_bonus
BONUS_EXE_DIR		:= $(BONUS_SRC_DIR)/executor_bonus
BONUS_BLT_DIR		:= $(BONUS_EXE_DIR)/builtins_bonus
BONUS_EXECVE_DIR	:= $(BONUS_EXE_DIR)/execve_ext_bonus
BONUS_LOC_VA_DIR	:= $(BONUS_EXE_DIR)/local_vars_bonus
BONUS_RED_PIP_DIR	:= $(BONUS_EXE_DIR)/redirection_pipe_bonus
BONUS_UTL_DIR		:= $(BONUS_SRC_DIR)/utils_bonus

# BONUS Source files
BONUS_SRCS		:= $(BONUS_SRC_DIR)/main_bonus.c \
			$(BONUS_UTL_DIR)/utils_bonus.c \
			$(BONUS_UTL_DIR)/herdoc_utils_bonus.c \
			$(BONUS_UTL_DIR)/init_utils_bonus.c \
			$(BONUS_UTL_DIR)/init_bonus.c \
			$(BONUS_UTL_DIR)/signal_bonus.c \
			$(BONUS_UTL_DIR)/error_bonus.c \
			$(BONUS_UTL_DIR)/free_bonus.c \
			$(BONUS_UTL_DIR)/histfile_utils_bonus.c \
			$(BONUS_UTL_DIR)/history_utils_bonus.c \
			$(BONUS_UTL_DIR)/history_bonus.c \
			$(BONUS_LEX_DIR)/ft_split_set_bonus.c \
			$(BONUS_LEX_DIR)/lexer_word_utils_0_bonus.c \
			$(BONUS_LEX_DIR)/lexer_word_utils_1_bonus.c \
			$(BONUS_LEX_DIR)/lexer_word_bonus.c \
			$(BONUS_LEX_DIR)/lexer_herdoc_utils_bonus.c \
			$(BONUS_LEX_DIR)/lexer_redir_filename_bonus.c \
			$(BONUS_LEX_DIR)/lexer_redir_utils_bonus.c \
			$(BONUS_LEX_DIR)/lexer_redir_bonus.c \
			$(BONUS_LEX_DIR)/glob_bonus.c \
			$(BONUS_LEX_DIR)/glob_utils_bonus.c \
			$(BONUS_LEX_DIR)/lexer_utils_bonus.c \
			$(BONUS_LEX_DIR)/lexer_bonus.c \
			$(BONUS_PAR_DIR)/parse_bonus.c \
			$(BONUS_PAR_DIR)/parse_redir_bonus.c \
			$(BONUS_PAR_DIR)/parser_utils_bonus.c \
			$(BONUS_PAR_DIR)/parser_error_bonus.c \
			$(BONUS_PAR_DIR)/parser_bonus.c \
			$(BONUS_BLT_DIR)/cd_bonus.c \
			$(BONUS_BLT_DIR)/echo_bonus.c \
			$(BONUS_BLT_DIR)/env_bonus.c \
			$(BONUS_BLT_DIR)/exit_bonus.c \
			$(BONUS_BLT_DIR)/pwd_bonus.c \
			$(BONUS_BLT_DIR)/unset_bonus.c \
			$(BONUS_BLT_DIR)/export_bonus.c \
			$(BONUS_BLT_DIR)/export_utils_0_bonus.c \
			$(BONUS_BLT_DIR)/export_utils_1_bonus.c \
			$(BONUS_EXE_DIR)/error_bonus.c \
			$(BONUS_EXE_DIR)/executor_bonus.c \
			$(BONUS_EXE_DIR)/utils_bonus.c \
			$(BONUS_LOC_VA_DIR)/local_utils_bonus.c \
			$(BONUS_LOC_VA_DIR)/exec_local_vars_bonus.c \
			$(BONUS_EXECVE_DIR)/execve_utils_bonus.c \
			$(BONUS_EXECVE_DIR)/execve_bonus.c \
			$(BONUS_RED_PIP_DIR)/pipe_bonus.c \
			$(BONUS_RED_PIP_DIR)/redirection_bonus.c

# Object files
OBJS		:= $(SRCS:.c=.o)
BONUS_OBJS		:= $(BONUS_SRCS:.c=.o)

# ******************************** RULES ************************************* #

COLOR_RESET := \033[0m
COLOR_GREEN := \033[1;32m
COLOR_BLUE  := \033[1;34m
COLOR_RED   := \033[1;31m

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(COLOR_BLUE)🔨 Linking $(NAME)...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@echo "$(COLOR_GREEN)✅ Successfully built $(NAME)!$(COLOR_RESET)"

$(OBJS): %.o: %.c $(INCLUDES)
	@echo "$(COLOR_BLUE)📦 Compiling $<...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT): $(LIBFT_SRCS) libft/libft.h
	@echo "$(COLOR_BLUE)📚 Building libft...$(COLOR_RESET)"
	@make -C libft

all:
	@echo "$(COLOR_BLUE)🚀 Building mandatory project...$(COLOR_RESET)"
	@$(MAKE) $(NAME)

$(BONUS_NAME): $(LIBFT) $(BONUS_OBJS)
	@echo "$(COLOR_BLUE)🔨 Linking $(BONUS_NAME)...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) $(BONUS_OBJS) $(LDFLAGS) -o $(BONUS_NAME)
	@echo "$(COLOR_GREEN)✅ Successfully built $(BONUS_NAME)!$(COLOR_RESET)"

$(BONUS_OBJS): %.o: %.c $(BONUS_INCLUDES)
	@echo "$(COLOR_BLUE)📦 Compiling $<...$(COLOR_RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

bonus:
	@echo "$(COLOR_BLUE)🚀 Building bonus project...$(COLOR_RESET)"
	@$(MAKE) $(BONUS_NAME)

clean:
	@echo "$(COLOR_RED)🧹 Cleaning object files...$(COLOR_RESET)"
	@make -C libft clean
	@rm -f $(OBJS)
	@rm -f $(BONUS_OBJS)
	@echo "$(COLOR_GREEN)✅ Clean done.$(COLOR_RESET)"

fclean: clean
	@echo "$(COLOR_RED)🧨 Full clean (executables)...$(COLOR_RESET)"
	@make -C libft fclean
	@rm -f $(NAME)
	@rm -f $(BONUS_NAME)
	@echo "$(COLOR_GREEN)✅ Full clean complete.$(COLOR_RESET)"

re: fclean all

.PHONY: all bonus clean fclean re
