/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 15:08:11 by lpupier           #+#    #+#             */
/*   Updated: 2023/04/05 12:40:47 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	ft_exit(char **cmd)
{
	long long	exit_status;

	exit_status = 0;
	printf("exit\n");
	if (get_array_size(cmd) > 2)
		return (printf("exit\e[31m: too many arguments\e[0m\n"), -1);
	if (cmd[1])
	{
		exit_status = ft_atoi(cmd[1]);
		if (exit_status == 2147483649)
			return (printf("exit\e[31m: numeric argument required\e[0m\n"), -1);
	}
	set_exit_status(exit_status);
	return (0);
}
