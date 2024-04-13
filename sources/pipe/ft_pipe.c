/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcart <vcart@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 12:16:45 by vcart             #+#    #+#             */
/*   Updated: 2023/03/30 14:44:31 by vcart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_pipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}

void	change_fd(int *fd, int value1, int value2)
{
	fd[0] = value1;
	fd[1] = value2;
}

int	exec_cmd(int *prev_fd, int *next_fd, char **cmd, t_env *env)
{
	if (prev_fd != NULL)
	{
		if (check_infiles(cmd))
		{
			if (handle_infiles(cmd, env, 0) == -1)
				return (-1);
		}
		else
		{
			close(prev_fd[1]);
			if (dup2(prev_fd[0], STDIN_FILENO) == -1)
				return (-1);
		}
	}
	if (next_fd != NULL)
	{
		if (check_next_fd(next_fd, cmd) == -1)
			return (-1);
	}
	check_functions(cmd, env, 1);
	return (get_exit_status());
}

int	handle_pipe(int *prev_fd, int *curr_fd, char ***cmd_tab, t_env *env)
{
	int	i;
	int	pid;
	int	num_pipes;

	i = -1;
	num_pipes = count_pipe(cmd_tab);
	while (++i < num_pipes)
	{
		if (i < num_pipes - 1)
		{
			if (pipe(curr_fd) == -1)
				return (-1);
		}
		pid = fork();
		if (pid < 0)
			return (-2);
		if (pid == 0)
			exit(exec_cmd(prev_fd, curr_fd, cmd_tab[i], env));
		if (prev_fd[0] != STDIN_FILENO)
			close_pipe(prev_fd);
		change_fd(prev_fd, curr_fd[0], curr_fd[1]);
	}
	return (pid);
}

int	ft_pipe(char ***cmd_tab, t_env *env)
{
	int	num_pipes;
	int	i;
	int	prev_fd[2];
	int	curr_fd[2];
	int	exit_status;

	num_pipes = count_pipe(cmd_tab);
	prev_fd[0] = STDIN_FILENO;
	prev_fd[1] = STDOUT_FILENO;
	waitpid(handle_pipe(prev_fd, curr_fd, cmd_tab, env), &exit_status, 0);
	set_exit_status(WEXITSTATUS(exit_status));
	close_pipe(prev_fd);
	i = 0;
	while (i < num_pipes)
	{
		wait(NULL);
		i++;
	}
	return (0);
}
