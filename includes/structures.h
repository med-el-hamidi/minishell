#ifndef STRUCTURES_H
# define STRUCTURES_H

/*
*
* Purpose: Tracks everything the shell needs during execution.
* env:            Copy of environment variables (like PATH, USER)
* exit_status:    Tracks $? (e.g., echo $? shows the last exit code).
*
*/
typedef struct s_shell
{
	char	**env;
	int		exit_status;
}	t_shell;

#endif
