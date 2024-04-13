/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcart <vcart@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/08 18:14:36 by vcart             #+#    #+#             */
/*   Updated: 2023/03/27 13:14:08 by vcart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	check_same_path(t_env *envi)
{
	char	cwd[1024];
	char	*oldpwd;

	oldpwd = get_env(envi->envp, "PWD");
	if (!oldpwd)
		return (-1);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (free(oldpwd), perror("getcwd"), -1);
	if (ft_strcmp(cwd, oldpwd))
		return (free(oldpwd), 1);
	return (free(oldpwd), 0);
}

static int	update_pwd(t_env *envi)
{
	char	cwd[1024];
	char	*cmd[3];
	char	*pwd;
	char	*wd;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("getcwd"), -1);
	cmd[0] = "export";
	pwd = ft_strdup("PWD=");
	if (!pwd)
		return (-1);
	wd = ft_strdup(cwd);
	if (!wd)
		return (free(pwd), -1);
	cmd[1] = ft_strjoin(pwd, wd);
	if (!cmd[1])
		return (free(pwd), free(wd), -1);
	cmd[2] = NULL;
	if (ft_export(cmd, envi->new_envp) == -1)
		return (free(pwd), free(wd), -1);
	free(cmd[1]);
	return (0);
}

static int	update_oldpwd(t_env *envi)
{
	char	cwd[1024];
	char	*cmd[3];
	char	*pwd;
	char	*wd;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (perror("getcwd"), -1);
	cmd[0] = "export";
	pwd = ft_strdup("OLDPWD=");
	if (!pwd)
		return (-1);
	wd = ft_strdup(cwd);
	if (!wd)
		return (free(pwd), -1);
	cmd[1] = ft_strjoin(pwd, wd);
	if (!cmd[1])
		return (free(pwd), free(wd), -1);
	cmd[2] = NULL;
	if (ft_export(cmd, envi->new_envp) == -1)
		return (free(pwd), free(wd), -1);
	free(cmd[1]);
	return (0);
}

int	cd(char **cmd, t_env *envi)
{
	int		argc;
	char	*home;

	argc = get_array_size(cmd);
	if (update_oldpwd(envi) == -1)
		return (-1);
	if (argc == 1)
	{
		home = get_env(envi->envp, "HOME");
		if (!home)
			return (-1);
		if (chdir(home) != 0)
			perror("cd");
		free(home);
	}
	else if (argc > 2)
		return (printf("cd : too many arguments\n"), 1);
	else if (chdir(cmd[1]) != 0)
		return (perror("cd"), 1);
	if (check_same_path(envi))
		return (update_pwd(envi));
	return (0);
}
