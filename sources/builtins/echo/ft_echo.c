/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 14:25:09 by lpupier           #+#    #+#             */
/*   Updated: 2023/03/14 14:40:05 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	display_str(char **cmd, int count)
{
	printf("%s", cmd[count]);
	if (count < get_array_size(cmd) - 1)
		printf(" ");
	return (1);
}

static int	args_gestion(char **cmd, int count, int *arg_error, int *no_return)
{
	int	idx;

	if (cmd[count][0] == '-' && *arg_error == 0)
	{
		idx = 1;
		while (cmd[count][idx])
		{
			if (cmd[count][idx] == 'n')
				idx++;
			else
			{
				*arg_error = display_str(cmd, count);
				break ;
			}
			if (!cmd[count][idx])
				*no_return = 1;
		}
		return (1);
	}
	else
		return (0);
}

int	echo(char **cmd)
{
	int	arg_error;
	int	no_return;
	int	count;

	if (!cmd[1])
	{
		printf("\n");
		return (0);
	}
	arg_error = 0;
	no_return = 0;
	count = 1;
	while (cmd[count])
	{
		if (!args_gestion(cmd, count, &arg_error, &no_return))
			arg_error = display_str(cmd, count);
		count++;
	}
	if (!no_return)
		printf("\n");
	return (0);
}
