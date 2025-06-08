#include "../includes/minishell.h"

/*---------------print_ast to Test AST------------------------------*/
static void print_ast(t_ast *node, int depth)
{
    if (!node) return;

    for (int i = 0; i < depth; i++) printf("  ");

    switch (node->type)
	{
        case AST_CMD:
            printf("COMMAND: ");
            for (int i = 0; node->args && node->args[i]; i++)
                printf("%s ", node->args[i]);
            printf("\n");
            break;
        case AST_PIPE:
            printf("PIPELINE\n");
            break;
        case AST_REDIR:
			if (node->redir_type == REDIR_INPUT)
            	printf("REDIR_IN: <%s>\n", node->redir_file);
			else if (node->redir_type == REDIR_OUTPUT)
            	printf("REDIR_OUT: <%s>\n", node->redir_file);
			else if (node->redir_type == REDIR_APPEND)
            	printf("REDIR_APPEND: <%s>\n", node->redir_file);
			else if (node->redir_type == REDIR_HEREDOC)
            	printf("REDIR_HEREDOC: <%s>\n", node->redir_file);
			else if (node->redir_type == REDIR_NONE)
            	printf("REDIR_NONE: <%s>\n", node->redir_file);
            break;
    }

    if (node->left)
        print_ast(node->left, depth + 1);
    if (node->right)
        print_ast(node->right, depth + 1);
}
/*--------------------------------------------------------------------*/

/*
*
* Main Function
* envp:	Environment pointer (array of environment variable strings), its format: "KEY=value" ex. PATH=/usr/bin:/bin
*
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	shell_loop(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
}

/*
* init_shell: Initialize minishell
*
* 1. Initialize environment variables (create copy)
* 2. Set default exit status & is_interactive
* 3. Backup standard file descriptors
* 4. Initialize signal handlers
* 5. Initialize history
*/
void init_shell(t_shell *shell, char **envp)
{
	shell->env_list = init_env(envp);
	if (!shell->env_list)
		exit_error("Failed to initialize the environment\n", 1);
	shell->exit_status = 0;
	shell->is_interactive = isatty(STDIN_FILENO);
	shell->stdin_fd = dup(STDIN_FILENO);
	shell->stdout_fd = dup(STDOUT_FILENO);
	if (shell->stdin_fd == -1 || shell->stdout_fd == -1)
		exit_error("Failed to backup std file descriptors", 0);
	setup_signals();
	if (shell->is_interactive)
		init_history(shell);
	init_termios(shell);
}

/*
* shell_loop: Program loop of Minishell
* 1. Display prompt and read input
* 2. Process non-empty input
* 3. Lexing → Parsing → Execution
* 4. Cleanup and repeat
*
*
*/
void shell_loop(t_shell *shell)
{
	char	*input;
	t_list	*tokens;
	t_ast	*ast;
	t_list	*ptr;

	while (1)  // REPL (Read-Eval-Print Loop)
	{
		input = readline("minishell$ ");
		if (!input)  // EOF (Ctrl+D)
			break;
		add_to_history(shell, input);  // Save to history
		tokens = lexer(shell, input);
		if (tokens)
		{
			shell->exit_status = 0;
			// --------------test lexer
			ptr = tokens;
			while (ptr)
			{
				printf("%d --> %s\n", ((t_token *)ptr->content)->type, ((t_token *)ptr->content)->value);
				ptr = ptr->next;
			}
			// ---------------end test
			ast = parser(tokens);
			if (ast)
			{
				print_ast(ast, 0);
				//shell->exit_status = executor(ast, shell);
				free_ast(ast);
			}
			ft_lstclear(&tokens, del_token);
		}
		free(input);
	}
}

void cleanup_shell(t_shell *shell)
{
	char	*path;

	path = get_history_path();
	if (path && (shell->history.count - shell->history.current) > 0)
			save_history(shell, path);
	free(path);
	free_2d_array(shell->history.entries);
	rl_clear_history();
	//cleanup_resources();
	ft_lstclear(&shell->env_list, del_env);
	close(shell->stdin_fd);
	close(shell->stdout_fd);
	if (shell->is_interactive)
		printf("exit\n");
}
