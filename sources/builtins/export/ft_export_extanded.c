/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_extanded.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcart <vcart@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:55:29 by vcart             #+#    #+#             */
/*   Updated: 2023/03/27 14:35:12 by vcart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	export_other_case(char **cmd, t_list *new_envp, int i)
{
	char	**cmd_split;

	cmd_split = ft_split(cmd[i], '=');
	if (!cmd_split)
		return (-1);
	check_all_cases(cmd, cmd_split, new_envp, i);
	return (0);
}

int	treat_plus_sign(char **cmd, char **cmd_split, \
t_list *new_envp, int i)
{
	int		j;
	char	*str_no_plus;

	str_no_plus = remove_plus(cmd_split[0]);
	if (!str_no_plus)
		return (-1);
	if (count_equals(cmd[i]) > 1)
	{
		j = 0;
		while (j < count_equals(cmd[i]))
		{
			(ft_list_find(new_envp, str_no_plus, 0)) \
			->content = ft_strjoin((ft_list_find(new_envp, \
			str_no_plus, 0))->content, ft_strdup("="));
			j++;
		}
	}
	(ft_list_find(new_envp, str_no_plus, 0)) \
	->content = ft_strjoin((ft_list_find(new_envp, \
	str_no_plus, 0))->content, cmd_split[1]);
	free(str_no_plus);
	free(cmd_split[0]);
	free(cmd_split);
	return (0);
}

int	export_new_value(char **cmd, char **cmd_split, t_list *new_envp\
, int i)
{
	t_list	*tmp;

	if (cmd_split[1][0] == '$')
	{
		if (ft_strcmp(cmd_split[1] + 1, cmd_split[0]))
		{
			if (treat_dollar_sign(new_envp, cmd_split) == -1)
				return (-1);
		}
	}
	else
	{
		tmp = (ft_list_find(new_envp, cmd_split[0], 0));
		free(tmp->content);
		tmp->content = ft_strdup(cmd[i]);
		if (!tmp->content)
			return (-1);
		free_tab(cmd_split);
	}
	return (0);
}

int	change_known_var(char **cmd, t_list *new_envp, int i)
{
	char	**cmd_split;
	char	*to_add;

	cmd_split = ft_split(cmd[i], '=');
	if (!cmd_split)
		return (-1);
	if (!ft_strcmp(cmd[i], ft_list_find(new_envp, cmd_split[0], 0)->content))
	{
		free_tab(cmd_split);
		return (0);
	}
	free(ft_list_find(new_envp, cmd_split[0], 0)->content);
	to_add = ft_strdup(cmd[i]);
	if (!to_add)
		return (free_tab(cmd_split), -1);
	ft_list_find(new_envp, cmd_split[0], 0)->content = to_add;
	if (!ft_list_find(new_envp, cmd_split[0], 0)->content)
		return (free_tab(cmd_split), -1);
	free_tab(cmd_split);
	return (0);
}

int	replace_dollar_value(t_list *envp, char **cmd_split)
{
	char	**elt;
	char	*elt_to_split;
	t_list	*tmp;

	if (ft_list_contains(envp, cmd_split[0], 0))
	{
		return (change_value(envp, cmd_split));
	}
	tmp = ft_list_find(envp, cmd_split[1] + 1, 0);
	if (!tmp)
		return (-1);
	elt_to_split = tmp->content;
	elt = ft_split(elt_to_split, '=');
	if (!elt)
		return (free(tmp), -1);
	free(tmp);
	free(elt_to_split);
	free(cmd_split[1]);
	cmd_split[0] = ft_strjoin(cmd_split[0], ft_strdup("="));
	if (!cmd_split[0])
		return (free_tab(elt), -1);
	cmd_split[1] = elt[1];
	ft_list_push_back(&envp, ft_strjoin(cmd_split[0], cmd_split[1]));
	free_tab(elt);
	return (0);
}
