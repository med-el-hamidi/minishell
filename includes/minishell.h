#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "structures.h"

/* Main Functions */
void	init_shell(t_shell *shell, char **envp);
void	shell_loop(t_shell *shell);
void	cleanup_shell(t_shell *shell);

#endif
