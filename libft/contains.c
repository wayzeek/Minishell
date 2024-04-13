/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   contains.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 13:23:47 by lpupier           #+#    #+#             */
/*   Updated: 2023/03/28 20:04:52 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	contains(char *str, char c)
{
	int		i;
	char	caracter;

	i = 0;
	caracter = 0;
	while (str[i])
	{
		if (str[i] == caracter)
			caracter = 0;
		else if (str[i] == '"' || str[i] == '\'')
			caracter = str[i];
		if (str[i] == c && caracter == 0)
			return (1);
		i++;
	}
	return (0);
}
