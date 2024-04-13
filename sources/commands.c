/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 21:19:19 by lpupier           #+#    #+#             */
/*   Updated: 2023/04/05 12:40:27 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Global_variable: exit status code
int	g_exit_status;

int	check_functions(char **cmd, t_env *envi, int status)
{
	int		builtins_exit;

	if (!cmd || !cmd[0])
		return (EXIT_SUCCESS);
	if (handle_infiles_and_redirections(cmd, envi) == -1)
		return (EXIT_SUCCESS);
	if (!check_infiles(cmd))
	{
		builtins_exit = check_builtins(cmd, envi, &g_exit_status);
		if (builtins_exit == -1)
			return (EXIT_FAILURE);
		if (builtins_exit == 0)
		{
			if (launch_program(cmd, status, envi) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		}
		if (!status)
		{
			if (dup2(0, STDOUT_FILENO) == -1)
				return (EXIT_FAILURE);
		}
	}
	if (put_fds_back(envi) == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	check_builtins(char **cmd, t_env *envi, int *exit_status)
{
	if (!cmd || !cmd[0])
		return (-1);
	if (!ft_strcmp(cmd[0], "echo"))
		*exit_status = echo(cmd);
	else if (!ft_strcmp(cmd[0], "pwd"))
		*exit_status = pwd(cmd);
	else if (!ft_strcmp(cmd[0], "env"))
		*exit_status = env(cmd, envi->new_envp);
	else if (!ft_strcmp(cmd[0], "cd"))
		*exit_status = cd(cmd, envi);
	else if (!ft_strcmp(cmd[0], "export"))
		*exit_status = ft_export(cmd, envi->new_envp);
	else if (!ft_strcmp(cmd[0], "unset"))
		*exit_status = ft_unset(cmd, envi->new_envp);
	else if (!ft_strcmp(cmd[0], "exit"))
	{
		if (ft_exit(cmd) != -1)
			return (-1);
	}
	else
		return (*exit_status = 127, 0);
	return (1);
}

int	launch_program(char **cmd, int status, t_env *envi)
{
	char	*path;
	int		child_status;

	path = get_binary_path(cmd[0], envi->envp);
	if (!path)
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd("\e[31m: Command not found.\e[0m\n", 2);
		return (EXIT_SUCCESS);
	}
	if (waitpid(binary(path, cmd, envi->envp), &child_status, 0) == -1)
		printf("\e[31m[ERROR]: %s\e[0m\n", cmd[0]);
	free(path);
	if (!status && check_redirections(cmd))
	{
		if (dup2(0, STDOUT_FILENO) == -1)
			return (EXIT_FAILURE);
		envi->fd_in = 1;
	}
	g_exit_status = WEXITSTATUS(child_status);
	return (EXIT_SUCCESS);
}

int	get_exit_status(void)
{
	return (g_exit_status);
}

void	set_exit_status(int code)
{
	g_exit_status = code;
}
