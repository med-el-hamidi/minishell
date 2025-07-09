/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 20:47:02 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 21:35:02 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile sig_atomic_t g_exit_status = 0;

void	init_shell(t_shell *shell, char **envp)
{
	shell->vars = NULL;
	shell->vars = init_env(envp);
	if (!shell->vars)
		_print_error("Failed to initialize the environment\n", 0);
	shell->exit_status = 0;
	shell->is_interactive = (shell->is_interactive && isatty(STDIN_FILENO));
	shell->stdin_fd = dup(STDIN_FILENO);
	shell->stdout_fd = dup(STDOUT_FILENO);
	if (shell->stdin_fd == -1 || shell->stdout_fd == -1)
		_print_error("Failed to backup std file descriptors", 0);
	setup_signals();
	if (shell->is_interactive)
	{
		init_history(shell);
		init_termios(shell);
	}
}

void	script_shell_loop(t_shell *shell, char *script)
{
	const int	fd = open_script(script);
	char		*input;
	t_list		*tokens;
	t_ast		*ast;

	while (1)
	{
		input = get_next_line(fd);
		if (!input)
			break ;
		tokens = lexer(shell, input);
		free(input);
		shell->tokens = &tokens;
		if (tokens)
		{
			shell->exit_status = 0;
			ast = parser(tokens);
			if (ast)
			{
				print_ast(ast, 0);//test
				shell->exit_status = executor(ast, shell);
				free_ast(ast);
			}
			ft_lstclear(&tokens, del_token);
		}
	}
	close(fd);
}

void	shell_loop(t_shell *shell)
{
	char	*input;
	t_list	*tokens;
	t_ast	*ast;

	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
			break ;
		add_to_history(shell, input);
		shell->exit_status = g_exit_status;
		tokens = lexer(shell, input);
		free(input);
		shell->tokens = &tokens;
		if (tokens)
		{
			g_exit_status = 0;
			shell->exit_status = 0;
			ast = parser(tokens);
			if (ast)
			{
				print_ast(ast, 0);//test
				shell->exit_status = executor(ast, shell);
				free_ast(ast);
			}
			ft_lstclear(&tokens, del_token);
		}
	}
}

void	cleanup_shell(t_shell *shell)
{
	if (shell->is_interactive)
	{
		if (shell->history.path
			&& !access(shell->history.path, F_OK | R_OK | W_OK)
			&& (shell->history.count - shell->history.current) > 0)
			save_history(shell, shell->history.path);
		free(shell->history.path);
		free_2d_array(shell->history.entries);
		rl_clear_history();
	}
	ft_lstclear(&shell->vars, del_env);
	ft_lstclear(&shell->vars, del_env);
	close(shell->stdin_fd);
	close(shell->stdout_fd);
	if (shell->is_interactive)
		printf("exit\n");
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	shell.is_interactive = 1;
	if (argv[1])
		shell.is_interactive = 0;
	init_shell(&shell, envp);
	if (!shell.is_interactive)
		script_shell_loop(&shell, argv[1]);
	else
		shell_loop(&shell);
	cleanup_shell(&shell);
	if (g_exit_status)
		return (g_exit_status);
	else
		return (shell.exit_status);
}
