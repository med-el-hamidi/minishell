//                     Conflict Zone
//                git pull -> instant update -> git push
//
// Hamidi: include minishell.h only in your files, your custom header files include them here but remember! look above
//
#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include "structures.h"

/* Main Functions */
void	init_shell(t_shell *shell, char **envp);
void	shell_loop(t_shell *shell);
void	cleanup_shell(t_shell *shell);

/* Error functions*/
void	exit_error(const char *msg, const int sys_error);

/* Init functions*/
char	**copy_env(char **envp);
void	setup_signals(void);

/* Utilities functions */
void	free_2d_array(char **arr);
#endif
