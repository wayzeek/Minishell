/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 13:46:49 by lpupier           #+#    #+#             */
/*   Updated: 2023/02/06 10:17:02 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdint.h>

void	*ft_calloc(size_t count, size_t size)
{
	char	*mem;
	size_t	idx;

	if (count == 0 || size == 0)
		return (malloc(0));
	if (SIZE_MAX / count < size)
		return (NULL);
	mem = malloc(sizeof(char) * (count * size));
	if (!mem)
		return (NULL);
	idx = 0;
	while (idx < (count * size))
	{
		mem[idx] = 0;
		idx++;
	}
	return ((void *)mem);
}
