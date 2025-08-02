#include "../../../includes/minishell.h"

static t_int_f	custom_ft_atoi(const char *str)
{
	long	nbr;
	long	tmp;
	int		sign;

	nbr = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		tmp = nbr;
		nbr = (nbr * 10) + (*str++ - '0');
		if ((nbr / 10) != tmp && sign == 1)
			return ((t_int_f){-1, 1});
		else if ((nbr / 10) != tmp && sign == -1)
			return ((t_int_f){0, 1});
	}
	return ((t_int_f){nbr * sign, 0});
}

static void	_exit_print_error(t_shell *sh, char *arg)
{
	sh->exit_status = 255;
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
}

static int	_finish_exit(char **argv, t_shell *sh)
{
	int		should_exit;
	t_int_f	exit_nbr;

	should_exit = 1;
	sh->exit_status = 0;
	if (isatty(STDOUT_FILENO) && isatty(STDIN_FILENO))
		ft_putendl_fd("exit", STDERR_FILENO);
	if (argv[1] && argv[2] && ft_strisnum(argv[1]))
	{
		sh->exit_status = 1;
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		should_exit = 0;
	}
	else if (argv[1] && ft_strisnum(argv[1]) == 0)
		_exit_print_error(sh, argv[1]);
	else if (argv[1])
	{
		exit_nbr = custom_ft_atoi(argv[1]);
		if ((!exit_nbr.nbr || exit_nbr.nbr == -1) && exit_nbr.f)
			_exit_print_error(sh, argv[1]);
		else
			sh->exit_status = (int)(exit_nbr.nbr & 0xFF);
	}
	return (should_exit);
}

void	builtin_exit(t_ast *ast, t_shell *sh)
{
	char	**args;

	args = ast->args;
	if (_finish_exit(args, sh))
	{
		free_ast(ast);
		ft_lstclear(sh->tokens, del_token);
		cleanup_shell(sh);
		if (g_exit_status)
			exit (g_exit_status);
		else
			exit (sh->exit_status);
	}
	return ;
}
