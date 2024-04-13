/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infiles_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcart <vcart@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:28:37 by vcart             #+#    #+#             */
/*   Updated: 2023/04/04 17:37:45 by vcart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_without_pipes(char **cmd, int i, t_env *env)
{
	char	**trunc_cmd;

	if (i > 0 && check_infiles(cmd) == 1)
	{
		trunc_cmd = ignore_infile(cmd);
		if (trunc_cmd)
		{
			check_functions(trunc_cmd, env, 1);
			free_tab(trunc_cmd);
		}
	}
	else if (i > 0 && check_infiles(cmd) == 2)
	{
		trunc_cmd = ignore_heredoc(cmd);
		if (trunc_cmd)
		{
			check_functions(trunc_cmd, env, 1);
			free_tab(trunc_cmd);
		}
	}
	else if (cmd + 2)
		check_functions(cmd + 2, env, 1);
	return (0);
}

int	create_heredoc(char **cmd, int i, int fd[2])
{
	char	*line;

	while (1)
	{
		line = NULL;
		ft_putstr_fd(">", 0);
		line = get_next_line(0);
		if (!line)
			return (ft_putstr_fd("bash: warning: here-document at line 15 delimited by \
end-of-file\n", 2), 0);
		if (!ft_strncmp(cmd[i + 1], line, ft_strlen(cmd[i + 1])))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd[1]);
		free(line);
	}
	return (0);
}

int	make_all_redirections(char **cmd, int i)
{
	int	fd;
	int	state;
	int	last_index;

	last_index = get_redirections_index(cmd);
	if (cmd[i + 1] == NULL || contains("<>", cmd[i + 1][0]))
		return (perror("minishell : syntax error near \
		unexpected token 'newline'"), -1);
	if (check_redirections(cmd) == 1)
		state = O_TRUNC;
	else if (check_redirections(cmd) == 2)
		state = O_APPEND;
	fd = open(cmd[i + 1], O_WRONLY | O_CREAT | state, 0644);
	if (fd == -1)
		return (perror("minishell : open"), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (-1);
	if (i != last_index)
		close(fd);
	return (fd);
}

int	put_fds_back(t_env *envi)
{
	if (envi->fd_in != 0)
	{
		close(envi->fd_in);
		if (envi->fd_out != 1)
		{
			if (dup2(envi->og_fd_in, 1) == -1)
				return (-1);
		}
		else
		{
			if (dup2(1, STDIN_FILENO) == -1)
				return (-1);
		}
		envi->fd_in = STDIN_FILENO;
	}
	if (envi->fd_out != 1)
	{
		close(envi->fd_out);
		if (dup2(envi->og_fd_out, 0) == -1)
			return (-1);
		envi->fd_out = STDOUT_FILENO;
	}
	return (0);
}

int	handle_infiles_and_redirections(char **cmd, t_env *envi)
{
	if (check_infiles(cmd))
	{
		if (handle_infiles(cmd, envi, 1) == -2)
			return (-1);
	}
	else if (check_redirections(cmd))
	{
		envi->fd_out = make_redirections(cmd);
		if (envi->fd_out == -1)
			return (-1);
	}
	return (0);
}
