/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:48:23 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 15:48:24 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*gen_tmp_file(void )
{
	static int	uniq_id = 0;
	char		*str;
	char		*tmp;
	char		*tmp_file;

	str = _getpid();
	tmp_file = ft_strjoin("/tmp/minishell_herdoc_", str);
	free(str);
	str = ft_itoa(uniq_id++);
	tmp = tmp_file;
	tmp_file = ft_strjoin(tmp_file, str);
	free(tmp);
	free(str);
	return (tmp_file);
}

void	set_herdoc_tmp_file(t_ast	*ast)
{
	char			*tmp_file;
	char			*s;
	int				fd;
	const size_t	l = ft_strlen(ast->redir_file);

	tmp_file = gen_tmp_file();
	fd = open(tmp_file, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (fd == -1)
		return (perror("minishell: herdoc"));
	while (1)
	{
		s = readline("> ");
		if (!s || (!ft_strncmp(s, ast->redir_file, l) && ft_strlen(s) == l))
		{
			if (!s)
				printf("minishell: warning: here-document delimited by end-of-file \
(wanted '%s')\n", ast->redir_file);
			break ;
		}
		ft_putendl_fd(s, fd);
		free(s);
	}
	free(ast->redir_file);
	ast->redir_file = tmp_file;
	close (fd);
}
