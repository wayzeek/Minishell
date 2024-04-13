/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcart <vcart@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 08:51:49 by vcart             #+#    #+#             */
/*   Updated: 2023/03/21 15:27:16 by vcart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_list	*ft_create_elem(void *data)
{
	t_list	*list;

	list = malloc(sizeof(t_list));
	if (list)
	{
		list->content = data;
		list->next = NULL;
	}
	return (list);
}

void	ft_list_push_back(t_list **begin_list, void *data)
{
	t_list	*elem;

	elem = *begin_list;
	while (elem->next)
		elem = elem->next;
	elem->next = ft_create_elem(data);
}

t_list	*ft_list_find(t_list *begin_list, void *data_ref, int search)
{
	char	**list_split;

	while (begin_list)
	{
		list_split = ft_split(begin_list->content, '=');
		if (!list_split)
			return (NULL);
		if (ft_strcmp(list_split[search], data_ref) == 0)
		{
			free_tab(list_split);
			return (begin_list);
		}
		free_tab(list_split);
		if (begin_list->next)
			begin_list = begin_list->next;
		else
			break ;
	}
	return (0);
}

int	ft_list_contains(t_list *begin_list, void *data_ref, int search)
{
	char	**list_split;

	while (begin_list)
	{
		if (search == 3)
		{
			if (ft_strcmp(begin_list->content, data_ref) == 0)
				return (1);
		}
		else
		{
			list_split = ft_split(begin_list->content, '=');
			if (!list_split)
				return (-1);
			if (ft_strcmp(list_split[search], data_ref) == 0)
				return (free_tab(list_split), 1);
			free_tab(list_split);
		}
		begin_list = begin_list->next;
	}
	return (0);
}

void	swap_nodes(t_list *a, t_list *b)
{
	char	*temp;

	temp = a->content;
	a->content = b->content;
	b->content = temp;
}
