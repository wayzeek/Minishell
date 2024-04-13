/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 18:19:49 by lpupier           #+#    #+#             */
/*   Updated: 2023/04/05 13:24:10 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	parse_quotes(char ***cmd, char *str, t_parsing *parsing)
{
	if (str[parsing->idx] == '"' || str[parsing->idx] == '\'')
	{
		while (str[parsing->idx] && (str[parsing->idx] == '"' \
				|| str[parsing->idx] == '\''))
		{
			parsing->c = str[parsing->idx];
			parsing->idx++;
			while (str[parsing->idx] && str[parsing->idx] != parsing->c)
				parsing->idx++;
			if (!str[parsing->idx])
				return (0);
			parse_heredoc(cmd, str, parsing);
			if (str[++parsing->idx] != '"' && str[parsing->idx] != '\'')
			{
				while (str[parsing->idx] && is_ws(str[parsing->idx]) == 0 \
				&& (str[parsing->idx] != '"' && str[parsing->idx] != '\''))
					parsing->idx++;
			}
		}
		*cmd = add_to_tab(*cmd, removing_quotes(get_var(\
		ft_substr(str, parsing->idx_init, parsing->idx - parsing->idx_init), \
				parsing->envp)));
		parsing->idx_init = parsing->idx + 1;
	}
	return (1);
}

char	*removing_quotes(char *str)
{
	char	*new_str;
	int		idx;
	int		len;
	char	caracter;

	idx = -1;
	len = 0;
	caracter = 0;
	while (str[++idx])
	{
		if (str[idx] == caracter)
			caracter = 0;
		else if ((str[idx] == '"' || str[idx] == '\'') && caracter == 0)
			caracter = str[idx];
		else
			len++;
	}
	new_str = malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (free(str), NULL);
	return (str_without_quote(str, new_str));
}

char	*str_without_quote(char *str, char *new_str)
{
	int		idx;
	int		new_idx;
	char	caracter;

	idx = -1;
	new_idx = 0;
	caracter = 0;
	while (str[++idx])
	{
		if (str[idx] == caracter)
			caracter = 0;
		else if ((str[idx] == '"' || str[idx] == '\'') && caracter == 0)
			caracter = str[idx];
		else
		{
			new_str[new_idx] = str[idx];
			new_idx++;
		}
	}
	new_str[new_idx] = '\0';
	return (free(str), new_str);
}
