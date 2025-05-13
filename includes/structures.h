//                     Conflict Zone
//                git pull -> instant update -> git push
//
#ifndef STRUCTURES_H
# define STRUCTURES_H

/*
*
* t_shell: Tracks everything the shell needs during execution.
*
* env:            Copy of environment variables (like PATH, USER)
* exit_status:    Tracks $? (e.g., echo $? shows the last exit code).
* is_interactive: Is the shell reading from a terminal?
* stdin_fd:       Backup of original stdin
* stdout_fd:      Backup of original stdout
*/
typedef struct s_shell
{
	char	**env;
	int		exit_status;
	int		is_interactive;
	int     stdin_fd;
	int     stdout_fd;
}	t_shell;

#endif
