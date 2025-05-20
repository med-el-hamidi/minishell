#ifndef EXEC_H
# define EXEC_H


int buildin_echo(char **argv);
int is_buildin(char *cmd);
int exec_buildin(t_ast *node, t_shell *shell);
int execute_ast(t_ast *node, t_shell *shell);


#endif