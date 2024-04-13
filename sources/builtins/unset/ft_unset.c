/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcart <vcart@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 10:07:53 by lpupier           #+#    #+#             */
/*   Updated: 2023/03/22 16:08:48 by vcart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	free_found_elt(t_list **head_lst, t_list *temp, char **to_compare)
{
	*head_lst = temp->next;
	free(temp->content);
	free(temp);
	free_tab(to_compare);
	return ;
}

int	ft_lstdelone(t_list **head_lst, char *data_ref)
{
	t_list	*temp;
	t_list	*prev;
	char	**to_compare;

	temp = *head_lst;
	to_compare = ft_split(temp->content, '=');
	if (!to_compare)
		return (-1);
	if (temp && !ft_strcmp(to_compare[0], data_ref))
		free_found_elt(head_lst, temp, to_compare);
	while (temp && ft_strcmp(to_compare[0], data_ref) != 0)
	{
		prev = temp;
		temp = temp->next;
		free_tab(to_compare);
		to_compare = ft_split(temp->content, '=');
		if (!to_compare)
			return (-1);
	}
	if (temp == NULL)
		return (0);
	prev->next = temp->next;
	free_tab(to_compare);
	return (free(temp->content), free(temp), 0);
}

int	ft_unset(char **cmd, t_list *new_envp)
{
	int		i;
	int		argc;

	argc = get_array_size(cmd);
	if (argc > 1)
	{
		i = 1;
		while (i < argc)
		{
			if (contains (cmd[i], '='))
			{
				printf("%s \e[31m: not a valid identifier\e[0m\n", cmd[i]);
				return (1);
			}
			else if (ft_list_contains(new_envp, cmd[i], 0))
			{
				if (ft_lstdelone(&new_envp, cmd[i]) == -1)
					return (-1);
			}
			i++;
		}
	}
	return (0);
}
