#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include "../libft/libft.h"
# include "structures.h"

/* Main Functions */
void	init_shell(t_shell *shell, char **envp);
void	shell_loop(t_shell *shell);
void	cleanup_shell(t_shell *shell);

/* Error functions*/
void	exit_error(char *msg, const int sys_error);

/* Init functions*/
t_list	*init_env(char **envp);
void	setup_signals(void);
void	init_history(t_shell *shell);
void	init_termios(t_shell *shell);

/* History functions*/
void	load_history(t_shell *shell);
void	add_to_history(t_shell *shell, char *input);
void	save_history(t_shell *shell);
void	load_recent_history(char *path, t_shell *shell, int histmem_lines_c, int histfile_lines_c);
void	load_history_fd(t_shell *shell, int	histfile_lines_c, int *skip, int fd);
int		get_histfile_lines_count(char *path, int oflag, int perm);

/* Free functions */
void	free_2d_array(char **arr);
void	del_env(void *v);
#endif
