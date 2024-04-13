/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcart <vcart@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 11:24:56 by vcart             #+#    #+#             */
/*   Updated: 2023/04/04 17:19:09 by vcart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	get_infiles_index(char **cmd)
{
	int	i;
	int	last_index;

	i = 0;
	last_index = -1;
	if (!check_infiles(cmd))
		return (-1);
	while (cmd[i] != NULL)
	{
		if (!ft_strcmp(cmd[i], "<") || !ft_strcmp(cmd[i], "<<"))
			last_index = i;
		i++;
	}
	return (last_index);
}

char	**ignore_infile(char **cmd)
{
	int		i;
	int		j;
	char	**new_cmd;

	i = -1;
	j = 0;
	new_cmd = malloc(sizeof(char *) * (get_array_size(cmd) + 1));
	if (!new_cmd)
		return (NULL);
	while (cmd[++i])
	{
		if (i > 0 && (!ft_strcmp(cmd[i], "<") || !ft_strcmp(cmd[i - 1], "<")))
			continue ;
		else if (ft_strcmp(cmd[i], "<"))
		{
			new_cmd[j] = ft_strdup(cmd[i]);
			if (!new_cmd[j])
				return (free_tab(new_cmd), NULL);
			j++;
		}
	}
	new_cmd[j] = NULL;
	return (new_cmd);
}

char	**ignore_heredoc(char **cmd)
{
	int		i;
	int		j;
	char	**new_cmd;

	i = -1;
	j = 0;
	new_cmd = malloc(sizeof(char *) * (get_array_size(cmd) + 1));
	if (!new_cmd)
		return (NULL);
	while (cmd[++i])
	{
		if (i > 0 && (!ft_strcmp(cmd[i], "<<") || !ft_strcmp(cmd[i - 1], "<<")))
			continue ;
		else if (ft_strcmp(cmd[i], "<<"))
		{
			new_cmd[j] = ft_strdup(cmd[i]);
			if (!new_cmd[j])
				return (free_tab(new_cmd), NULL);
			j++;
		}
	}
	new_cmd[j] = NULL;
	return (new_cmd);
}

int	handle_infiles(char **cmd, t_env *env, int status)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (!ft_strcmp(cmd[i], "<") || !ft_strcmp(cmd[i], "<<"))
		{
			if (handle_both_infiles(cmd, env, status, i) == -2)
			{
				if (dup2(1, STDIN_FILENO) == -1)
					return (-1);
				return (-2);
			}
		}
		i++;
	}
	return (1);
}

int	handle_heredoc(char **cmd, int status, int i)
{
	int		last_index;
	int		fd[2];

	last_index = get_infiles_index(cmd);
	if (check_infiles(cmd) == 2)
	{
		if (pipe(fd) == -1)
			return (perror("minishell : pipe"), -1);
		if (status)
		{
			if (create_heredoc(cmd, i, fd) == -1)
				return (-1);
		}
		close(fd[1]);
		if (status == 1 && i == last_index)
		{
			if (dup2(fd[0], STDIN_FILENO) == -1)
				return (-1);
		}
		return (fd[0]);
	}
	return (0);
}
