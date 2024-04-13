/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcart <vcart@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 10:55:19 by vcart             #+#    #+#             */
/*   Updated: 2023/03/27 16:01:29 by vcart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	treat_empty_value_condition(char **cmd, char *export_cmd, \
											t_list *new_envp, int i)
{
	char	**split_cmd;
	char	*to_add;
	t_list	*tmp;

	split_cmd = ft_split(export_cmd, '=');
	if (!split_cmd)
		return (-1);
	to_add = ft_strjoin(ft_strdup(cmd[i]), ft_strdup(""));
	if (!to_add)
		return (free_tab(split_cmd), -1);
	if (!ft_list_contains(new_envp, split_cmd[0], 0))
		ft_lstadd_back(&new_envp, ft_lstnew(to_add));
	else
	{
		tmp = ft_list_find(new_envp, split_cmd[0], 0);
		free(tmp->content);
		tmp->content = to_add;
	}
	free_tab(split_cmd);
	return (0);
}

int	count_plus(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '+')
			count++;
		i++;
	}
	return (count);
}

int	get_equal_index(char *export_cmd)
{
	int	i;

	i = 0;
	while (export_cmd[i])
	{
		if (export_cmd[i] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	treat_empty_value(char **cmd, char *export_cmd, t_list *new_envp, int cmd_i)
{
	int		i;
	char	**cmd_split;

	i = get_equal_index(export_cmd);
	cmd_split = ft_split(export_cmd, '=');
	if (!cmd_split)
		return (-1);
	if (i == -1)
	{
		if (!ft_list_contains(new_envp, export_cmd, 0))
			ft_lstadd_back(&new_envp, ft_lstnew(ft_strdup(export_cmd)));
		return (free_tab(cmd_split), 1);
	}
	else if (i == 0)
	{
		printf("%s \e[31m: not a valid identifier\e[0m\n", export_cmd);
		return (free_tab(cmd_split), 1);
	}
	else if (export_cmd[i + 1] == '\0' || !cmd_split[1])
	{
		if (treat_empty_value_condition(cmd, export_cmd, new_envp, cmd_i) == -1)
			return (free_tab(cmd_split), -1);
		return (free_tab(cmd_split), 1);
	}
	return (free_tab(cmd_split), 0);
}

void	free_list(t_list *list)
{
	t_list	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->content);
		free(tmp);
	}
}
