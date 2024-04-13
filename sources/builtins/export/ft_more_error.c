/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_more_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcart <vcart@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 20:21:58 by vcart             #+#    #+#             */
/*   Updated: 2023/03/27 15:37:35 by vcart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	check_forbidden_char(char *str)
{
	int	len;
	int	i;

	len = ft_strlen(str);
	i = 0;
	while (i < len - 1)
	{
		if (!ft_isalpha(str[i]))
			return (1);
		i++;
	}
	return (0);
}

int	count_equals(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '=')
			count++;
		i++;
	}
	return (count);
}

int	treat_multiple_equals(char *export_cmd, t_list *new_envp)
{
	char	**cmd_split;
	t_list	*temp;

	cmd_split = ft_split(export_cmd, '=');
	if (!cmd_split)
		return (-1);
	if (!ft_list_contains(new_envp, cmd_split[0], 0))
		ft_lstadd_back(&new_envp, ft_lstnew(ft_strdup(export_cmd)));
	else
	{
		temp = (ft_list_find(new_envp, cmd_split[0], 0));
		free(temp->content);
		temp->content = ft_strdup(export_cmd);
		if (!temp->content)
			return (free_tab(cmd_split), -1);
	}
	free_tab(cmd_split);
	return (0);
}
