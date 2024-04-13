/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_expansion.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vcart <vcart@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:59:55 by vcart             #+#    #+#             */
/*   Updated: 2023/03/27 15:58:26 by vcart            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	print_export_equals(t_list *new_envp)
{
	int		i;
	char	*elt;

	i = 0;
	elt = (char *)new_envp->content;
	printf("declare -x ");
	while (elt[i] != '=')
	{
		printf("%c", elt[i]);
		i++;
	}
	printf("=\"%s\"\n", elt + i + 1);
}

int	check_prompt_error(char *cmd, char **cmd_split)
{
	if (count_plus(cmd_split[0]) > 1)
	{
		printf("%s \e[31m: not a valid identifier\e[0m\n", cmd);
		return (-1);
	}
	else if (check_forbidden_char(cmd_split[0]))
	{
		printf("%s \e[31m: not a valid identifier\e[0m\n", cmd);
		return (-1);
	}
	else if (contains("0123456789", cmd_split[0][0]) || \
			contains(cmd_split[0], '-'))
	{
		printf("%s \e[31m: not a valid identifier\e[0m\n", cmd);
		return (-1);
	}
	return (0);
}

int	change_value(t_list *envp, char **cmd_split)
{
	char	*cmd[3];

	cmd[0] = "unset";
	cmd[1] = cmd_split[0];
	cmd[2] = NULL;
	if (ft_unset(cmd, envp) == -1)
		return (-1);
	return (0);
}

int	treat_different_cases(char **cmd, t_list *new_envp, int i)
{
	if (cmd[i + 1] && cmd[i + 1][0] == '=')
	{
		cmd[i] = ft_strjoin(cmd[i], cmd[i + 1]);
		cmd[i + 1] = NULL;
	}
	if (check_export_error(cmd[i]) == -1 || \
	treat_empty_value(cmd, cmd[i], new_envp, i) == 1)
		return (0);
	else if (ft_list_contains(new_envp, cmd[i], 3) && contains(cmd[i], '='))
	{
		if (change_known_var(cmd, new_envp, i) == -1)
			return (-1);
	}
	else if (!ft_list_contains(new_envp, cmd[i], 3) && \
	!contains(cmd[i], '='))
		ft_list_push_back(&new_envp, ft_strdup(cmd[i]));
	else
	{
		if (export_other_case(cmd, new_envp, i) == -1)
			return (-1);
	}
	return (0);
}

int	check_all_cases(char **cmd, char **cmd_split, t_list *new_envp, int i)
{
	char	*str_no_plus;

	str_no_plus = remove_plus(cmd_split[0]);
	if (!str_no_plus)
		return (-1);
	if (cmd_split[0][ft_strlen(cmd_split[0]) - 1] == '+' && \
	ft_list_contains(new_envp, str_no_plus, 0))
	{
		if (treat_plus_sign(cmd, cmd_split, new_envp, i) == -1)
			return (free(str_no_plus), -1);
	}
	else if (!ft_list_contains(new_envp, cmd_split[0], 0))
	{
		if (treat_special_case(cmd, new_envp, cmd_split, i) == -1)
			return (free(str_no_plus), -1);
	}
	else
	{
		if (export_new_value(cmd, cmd_split, new_envp, i) == -1)
			return (free(str_no_plus), -1);
	}
	free(str_no_plus);
	return (0);
}
