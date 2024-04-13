/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_check_export.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 15:18:47 by vcart             #+#    #+#             */
/*   Updated: 2023/03/27 16:46:30 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	check_export_error(char *cmd)
{
	char	**cmd_split;

	if (ft_strlen(cmd) == 1 && !ft_isalpha(cmd[0]))
		return (printf("%s \e[31m: not a valid identifier\e[0m\n", cmd), -1);
	else if (count_equals(cmd) == (int)ft_strlen(cmd))
		return (printf("%s \e[31m: not a valid identifier\e[0m\n", cmd), -1);
	else
	{
		cmd_split = ft_split(cmd, '=');
		if (!cmd_split)
			return (-1);
		if (check_prompt_error(cmd, cmd_split) == -1)
			return (free_tab(cmd_split), -1);
		else if (ft_strlen(cmd_split[0]) == 1 && \
		!ft_isalpha(cmd_split[0][0]))
			return (printf("%s \e[31m: not a valid identifier\e[0m\n", \
			cmd), free_tab(cmd_split), -1);
		else if (cmd_split[1] && (contains(cmd_split[1], '(') \
			|| contains(cmd_split[1], ')')))
			return (printf("%s \e[31m: not a valid identifier\e[0m\n", \
			cmd), free_tab(cmd_split), -1);
		free_tab(cmd_split);
	}
	return (0);
}

int	print_export(t_list *new_envp)
{
	char	**envp_split;

	sort_envp(new_envp);
	while (new_envp)
	{
		if (get_equal_index(new_envp->content) == -1)
			printf("declare -x %s\n", (char *)new_envp->content);
		else if (count_equals(new_envp->content) > 1)
			print_export_equals(new_envp);
		else if (ft_strncmp(new_envp->content, "_=", 2) != 0)
		{
			envp_split = ft_split(new_envp->content, '=');
			if (!envp_split)
				return (-1);
			if (envp_split[1])
				printf("declare -x %s=\"%s\"\n", envp_split[0], envp_split[1]);
			else if (!envp_split[1])
				printf("declare -x %s=\"\"\n", envp_split[0]);
			free_tab(envp_split);
		}
		new_envp = new_envp->next;
	}
	return (0);
}

int	treat_dollar_sign(t_list *envp, char **cmd_split)
{
	if (cmd_split[1][0] == '$' && \
	ft_list_contains(envp, cmd_split[1] + 1, 0))
	{
		if (replace_dollar_value(envp, cmd_split) == -1)
			return (-1);
	}
	else if (cmd_split[1][0] == '$' && \
	!ft_list_contains(envp, cmd_split[1] + 1, 0))
	{
		cmd_split[0] = ft_strjoin(cmd_split[0], ft_strdup("="));
		cmd_split[1] = ft_strdup("");
		if (!cmd_split[0] || !cmd_split[1])
			return (-1);
		ft_list_push_back(&envp, ft_strjoin(cmd_split[0], cmd_split[1]));
	}
	return (0);
}

int	treat_special_case(char **cmd, t_list *envp, char **cmd_split, int i)
{
	if (count_equals(cmd[i]) > 1)
	{
		if (treat_multiple_equals(cmd[i], envp) == -1)
			return (-1);
	}
	else if (cmd_split[1][0] == '$')
	{
		if (ft_strcmp(cmd_split[1] + 1, cmd_split[0]))
		{
			if (treat_dollar_sign(envp, cmd_split) == -1)
				return (-1);
		}
	}
	else if (cmd_split[0][ft_strlen(cmd_split[0]) - 1] == '+')
		ft_list_push_back(&envp, remove_plus(cmd[i]));
	else
		ft_list_push_back(&envp, ft_strdup(cmd[i]));
	free_tab(cmd_split);
	return (0);
}

int	treat_export(char **cmd, t_list *new_envp, int argc)
{
	int		i;

	i = 1;
	while (i < argc && cmd[i])
	{
		if (treat_different_cases(cmd, new_envp, i) == -1)
			return (-1);
		i++;
	}
	return (0);
}
