/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 17:26:01 by lpupier           #+#    #+#             */
/*   Updated: 2023/04/05 13:32:39 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

pid_t	binary(const char *program, char **args, char **envp)
{
	pid_t	pid;

	signal(SIGINT, signal_ctrl_c_child);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
		return (pid);
	else
	{
		if (execve(program, args, envp) == -1)
		{
			ft_putstr_fd((char *)program, 2);
			ft_putstr_fd("\e[31m: Command not found.\e[0m\n", 2);
			exit(127);
		}
		exit(EXIT_FAILURE);
	}
}

int	is_in_envp(char **envp, char *request)
{
	int		i;
	char	**env_split;

	i = 0;
	while (envp[i])
	{
		env_split = ft_split(envp[i], '=');
		if (!env_split)
			return (-1);
		if (!ft_strcmp(env_split[0], request))
			return (free_tab(env_split), 1);
		free_tab(env_split);
		i++;
	}
	return (0);
}

char	*get_env(char **envp, char *request)
{
	int		i;
	char	**env_split;
	char	*result;

	i = 0;
	if (!is_in_envp(envp, request))
		return (NULL);
	while (envp[i])
	{
		env_split = ft_split(envp[i], '=');
		if (!env_split)
			return (NULL);
		if (!env_split[1])
			return (free(env_split[0]), free(env_split), NULL);
		if (!ft_strcmp(env_split[0], request))
		{
			result = ft_strdup(env_split[1]);
			if (!result)
				return (free_tab(env_split), NULL);
			return (free_tab(env_split), result);
		}
		free_tab(env_split);
		i++;
	}
	return (NULL);
}

static char	*check_existing_binary(char *cmd, char **path_list, int i)
{
	char	*path;
	char	*result;

	path = ft_strjoin(ft_strdup(path_list[i]), ft_strdup("/"));
	if (!path)
		return (free_tab(path_list), NULL);
	result = ft_strjoin(path, ft_strdup(cmd));
	if (!result)
		return (free(path), free_tab(path_list), NULL);
	if (access(result, F_OK | X_OK) == 0)
		return (free_tab(path_list), result);
	free(result);
	return (NULL);
}

char	*get_binary_path(char *cmd, char **envp)
{
	char	**path_list;
	char	*result;
	char	*env;
	int		i;

	env = get_env(envp, "PATH");
	if (!env)
		return (NULL);
	path_list = ft_split(env, ':');
	if (!path_list)
		return (free(env), NULL);
	free(env);
	i = -1;
	while (path_list[++i])
	{
		result = check_existing_binary(cmd, path_list, i);
		if (result)
			return (result);
	}
	return (free_tab(path_list), ft_strdup(cmd));
}
