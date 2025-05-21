#ifndef EXEC_H
# define EXEC_H


int builtin_echo(char **argv);
int builtin_cd(char **argv);
int is_builtin(char *cmd);
int exec_builtins(t_ast *node, t_shell *shell);
int execute_ast(t_ast *node, t_shell *shell);


#endif