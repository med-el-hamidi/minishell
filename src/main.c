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

volatile sig_atomic_t	g_exit_status = 0;

void	init_shell(t_shell *shell, char **envp)
{
	shell->vars = NULL;
	shell->vars = init_env(shell->argv[0], envp);
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

	shell->tokens = &tokens;
	while (1)
	{
		input = get_next_line(fd);
		if (!input)
			break ;
		tokens = lexer(shell, input);
		free(input);
		if (tokens)
		{
			ast = parser(tokens, shell);
			if (ast)
			{
				update_lastarg_var(tokens, shell->vars);
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

	shell->tokens = &tokens;
	while (1)
	{
		input = readline(shell->prompt);
		if (!input)
			break ;
		add_to_history(shell, input);
		tokens = lexer(shell, input);
		free(input);
		if (tokens)
		{
			ast = parser(tokens, shell);
			if (ast)
			{
				update_lastarg_var(tokens, shell->vars);
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
			&& (shell->history.count - shell->history.current) > 0)
			save_history(shell, shell->history.path);
		free(shell->history.path);
		free_2d_array(shell->history.entries);
		rl_clear_history();
	}
	free(shell->name);
	free(shell->prompt);
	ft_lstclear(&shell->vars, del_env);
	close(shell->stdin_fd);
	close(shell->stdout_fd);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	shell.argc = argc;
	shell.argv = argv;
	if (ft_strrchr(argv[0], '/'))
		shell.name = ft_substr(argv[0], \
			ft_strrchr(argv[0], '/') - argv[0] + 1, ft_strlen(argv[0]));
	else
		shell.name = ft_strdup(argv[0]);
	shell.prompt = ft_strjoin(shell.name, "$ ");
	shell.is_interactive = 1;
	if (argv[1])
		shell.is_interactive = 0;
	init_shell(&shell, envp);
	if (!shell.is_interactive)
		script_shell_loop(&shell, argv[1]);
	else
		shell_loop(&shell);
	cleanup_shell(&shell);
	if (shell.is_interactive)
		printf("exit\n");
	if (g_exit_status)
		return (g_exit_status);
	else
		return (shell.exit_status);
}
