/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_extended.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcart <vcart@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:14:26 by vcart             #+#    #+#             */
/*   Updated: 2023/04/04 17:36:56 by vcart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_next_fd(int *next_fd, char **cmd)
{
	if (check_redirections(cmd))
	{
		if (make_redirections(cmd) == -1)
			return (-1);
	}
	else
	{
		close(next_fd[0]);
		if (dup2(next_fd[1], STDOUT_FILENO) == -1)
			return (-2);
		close(next_fd[1]);
	}
	return (0);
}

int	open_infile(char **cmd, int i)
{
	int	fd;
	int	last_index;

	last_index = get_infiles_index(cmd);
	fd = open(cmd[i + 1], O_RDONLY);
	if (fd == -1)
		return (perror("minishell : open"), -2);
	if (i == last_index)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
			return (-2);
		return (fd);
	}
	close(fd);
	return (0);
}

void	free_redirections(int i, char **cmd)
{
	int	size;

	size = get_array_size(cmd);
	while (i < size)
	{
		free(cmd[i]);
		cmd[i] = NULL;
		i++;
	}
}

int	handle_both_infiles(char **cmd, t_env *env, int status, int i)
{
	int	last_index;

	last_index = get_infiles_index(cmd);
	if ((i == 0 && cmd[i + 1] == NULL) || contains("<>", cmd[i + 1][0]))
		return (perror("minishell : syntax error near unexpected\
token `newline'"), -2);
	if (!ft_strcmp(cmd[i], "<"))
	{
		env->fd_in = open_infile(cmd, i);
		if (env->fd_in == -2)
			return (-2);
	}
	else if (!ft_strcmp(cmd[i], "<<"))
	{
		env->fd_in = handle_heredoc(cmd, status, i);
		if (env->fd_in == -1)
			return (-2);
	}
	if (status == 1 && i == last_index)
	{
		if (handle_without_pipes(cmd, i, env) == -1)
			return (-2);
	}
	return (0);
}
