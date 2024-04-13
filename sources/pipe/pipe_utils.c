/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcart <vcart@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/13 11:10:19 by vcart             #+#    #+#             */
/*   Updated: 2023/04/04 10:52:30 by vcart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_pipe(char ***cmd_tab)
{
	int	nb_pipe;

	nb_pipe = 0;
	while (cmd_tab[nb_pipe] != NULL)
		nb_pipe++;
	return (nb_pipe);
}

int	check_redirections(char **cmd)
{
	int	i;
	int	last_index;

	i = 0;
	last_index = 0;
	while (cmd[i] != NULL)
	{
		if (!ft_strcmp(cmd[i], ">"))
			last_index = 1;
		else if (!ft_strcmp(cmd[i], ">>"))
			last_index = 2;
		i++;
	}
	return (last_index);
}

int	check_infiles(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i] != NULL)
	{
		if (!ft_strcmp(cmd[i], "<"))
			return (1);
		else if (!ft_strcmp(cmd[i], "<<"))
			return (2);
		i++;
	}
	return (0);
}

int	get_redirections_index(char **cmd)
{
	int	i;

	i = 0;
	if (!check_redirections(cmd))
		return (-1);
	while (cmd[i] != NULL)
	{
		if (!ft_strcmp(cmd[i], ">") || !ft_strcmp(cmd[i], ">>"))
			return (i);
		i++;
	}
	return (0);
}

int	make_redirections(char **cmd)
{
	int	i;
	int	last_index;
	int	fd_out;

	i = 0;
	last_index = get_redirections_index(cmd);
	fd_out = 1;
	while (cmd[i])
	{
		if (!ft_strcmp(cmd[i], ">") || !ft_strcmp(cmd[i], ">>"))
		{
			fd_out = make_all_redirections(cmd, i);
			if (fd_out == -1)
				return (-1);
		}
		i++;
	}
	free_redirections(last_index, cmd);
	return (fd_out);
}
