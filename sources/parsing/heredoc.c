/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 14:43:56 by lpupier           #+#    #+#             */
/*   Updated: 2023/03/29 16:47:04 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_heredoc(char ***cmd, char *str, t_parsing *parsing)
{
	char	*heredoc;

	heredoc = is_heredoc(str + parsing->idx);
	if (heredoc)
	{
		*cmd = add_to_tab(*cmd, get_var(ft_substr(str, parsing->idx_init, \
							parsing->idx - parsing->idx_init), parsing->envp));
		*cmd = add_to_tab(*cmd, ft_strdup(heredoc));
		parsing->idx_init = parsing->idx + ft_strlen(heredoc);
		if (ft_strlen(heredoc) == 2)
			parsing->idx++;
		return (1);
	}
	return (0);
}

char	*is_heredoc(char *str)
{
	if (!ft_strncmp(str, ">>", 2))
		return (">>");
	else if (!ft_strncmp(str, "<<", 2))
		return ("<<");
	else if (!ft_strncmp(str, ">", 1))
		return (">");
	else if (!ft_strncmp(str, "<", 1))
		return ("<");
	return (NULL);
}

char	**reorganize_tab_heredoc(char **cmd)
{
	int		count_loop;
	int		len_tab;
	char	**new_cmd;

	count_loop = -1;
	while (++count_loop < nb_loop_heredoc(cmd))
	{
		len_tab = get_array_size(cmd);
		new_cmd = malloc(sizeof(char *) * (len_tab + 1));
		if (!new_cmd)
			return (NULL);
		new_cmd[len_tab] = NULL;
		loop_reorganize_heredoc(cmd, &new_cmd, len_tab);
		free_tab(cmd);
		cmd = new_cmd;
	}
	return (cmd);
}

int	nb_loop_heredoc(char **cmd)
{
	int	count;
	int	idx;

	count = 0;
	idx = -1;
	while (cmd[++idx])
	{
		if (is_heredoc(cmd[idx]))
			count++;
	}
	if (count > 1)
		return (2);
	return (1);
}

void	loop_reorganize_heredoc(char **cmd, char ***new_cmd, int len_tab)
{
	int	idx;
	int	new_idx;
	int	max_idx;

	max_idx = len_tab - 1;
	idx = -1;
	new_idx = 0;
	while (cmd[++idx])
	{
		if (is_heredoc(cmd[idx]))
		{
			if (cmd[idx + 1])
			{
				(*new_cmd)[max_idx] = ft_strdup(cmd[idx + 1]);
				(*new_cmd)[max_idx - 1] = ft_strdup(cmd[idx]);
				max_idx -= 1;
				idx++;
			}
			else
				(*new_cmd)[max_idx] = ft_strdup(cmd[idx]);
			max_idx -= 1;
		}
		else
			(*new_cmd)[new_idx++] = ft_strdup(cmd[idx]);
	}
}
