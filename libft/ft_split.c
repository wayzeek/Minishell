/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 10:51:30 by lpupier           #+#    #+#             */
/*   Updated: 2023/04/05 13:55:12 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_count_separator(char const *s, char c)
{
	size_t	idx;
	size_t	count;
	char	caracter;

	idx = 0;
	count = 0;
	caracter = 0;
	while (s[idx])
	{
		if (s[idx++] != c)
		{
			count++;
			while (s[idx] && (s[idx] != c || caracter != 0))
			{
				if (s[idx] == caracter)
					caracter = 0;
				else if (s[idx] == '"' || s[idx] == '\'')
					caracter = s[idx];
				idx++;
			}
		}
	}
	return (count);
}

static void	*ft_free_tab(char **tab)
{
	while (*tab != NULL)
	{
		free(*tab);
		tab++;
	}
	free(tab);
	return (NULL);
}

static size_t	ft_find_end(char const *s, size_t start, char c)
{
	size_t	idx;
	char	caracter;

	idx = start;
	caracter = 0;
	while (s[idx])
	{
		if (s[idx] == caracter)
			caracter = 0;
		else if (s[idx] == '"' || s[idx] == '\'')
			caracter = s[idx];
		if (s[idx] == c && caracter == 0)
			break ;
		idx++;
	}
	return (idx);
}

static char	**ft_assign_value(char **tab, char const *s, char c)
{
	size_t	idx;
	size_t	idx_start;
	size_t	idx_tab;

	idx = 0;
	idx_tab = 0;
	while (s[idx])
	{
		if (s[idx] != c)
		{
			idx_start = idx;
			idx = ft_find_end(s, idx_start, c);
			tab[idx_tab] = ft_substr(s, idx_start, idx - idx_start);
			if (!tab[idx_tab])
				return (ft_free_tab(tab));
			idx_tab++;
		}
		else
			idx++;
	}
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	size_t	count;
	char	**tab;

	if (!s)
		return (NULL);
	count = ft_count_separator(s, c);
	tab = malloc(sizeof(char *) *(count + 1));
	if (!tab)
		return (NULL);
	tab[count] = NULL;
	tab = ft_assign_value(tab, s, c);
	return (tab);
}
