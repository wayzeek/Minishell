/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 12:46:04 by lpupier           #+#    #+#             */
/*   Updated: 2023/03/28 19:01:02 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	signal_ctrl_c(int id)
{
	(void)id;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	signal_ctrl_c_child(int id)
{
	(void)id;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	signal_ctrl_backslash(int id)
{
	(void)id;
}
