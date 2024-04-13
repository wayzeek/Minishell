/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/27 17:13:47 by lpupier           #+#    #+#             */
/*   Updated: 2023/04/04 05:03:26 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_var(char *str, char **envp)
{
	int		idx;
	int		is_quote;

	idx = -1;
	is_quote = 0;
	while (str[++idx])
	{
		if (str[idx] == '\'' && (is_quote || contains(str + idx, '\'')))
			is_quote = !is_quote;
		if (str[idx] == '$' && is_quote == 0)
		{
			idx = replace_var(envp, &str, idx + 1, idx + 1);
			if (idx >= (int)ft_strlen(str))
				break ;
		}
	}
	return (str);
}

int	replace_var(char **envp, char **cmd, int idx, int new_idx)
{
	char	*var_env;
	char	*new_cmd;

	var_env = get_var_init(envp, cmd, idx, &new_idx);
	if (var_env)
		new_cmd = ft_strjoin(ft_strjoin(ft_substr(*cmd, 0, idx - 1), \
		var_env), ft_substr(*cmd, new_idx, ft_strlen(*cmd)));
	else
	{
		if (ft_isdigit((*cmd)[idx]))
			new_cmd = ft_strjoin(ft_substr(*cmd, 0, idx - 1), \
			ft_substr(*cmd, idx + 1, ft_strlen(*cmd)));
		else
		{
			new_cmd = ft_strjoin(ft_substr(*cmd, 0, idx - 1), \
			ft_substr(*cmd, new_idx, ft_strlen(*cmd)));
			new_idx = idx - 2;
			if (new_idx < 0)
				new_idx = 0;
		}
	}
	if (ft_strlen(*cmd) - ft_strlen(new_cmd) == 1)
		return (free(*cmd), *cmd = new_cmd, new_idx - 1);
	return (free(*cmd), *cmd = new_cmd, new_idx - 1);
}

char	*get_var_init(char **envp, char **cmd, int idx, int *new_idx)
{
	char	*var;
	char	*var_env;

	if ((*cmd)[*new_idx] == '\0')
		var_env = ft_strdup("$");
	else if ((*cmd)[*new_idx] == '?')
	{
		var_env = ft_itoa(get_exit_status());
		*new_idx += 1;
	}
	else
	{
		while ((*cmd)[*new_idx] && \
		(ft_isalnum((*cmd)[*new_idx]) || (*cmd)[*new_idx] == '_'))
			*new_idx += 1;
		var = ft_substr(*cmd, idx, *new_idx - idx);
		var_env = get_env(envp, var);
		if (!var_env)
			return (free(var), NULL);
		free(var);
	}
	return (var_env);
}

char	**join_2_tab(char **cmd1, char **cmd2)
{
	char	**new_cmd;
	int		size;
	int		idx;
	int		new_idx;

	size = get_array_size(cmd1) + get_array_size(cmd2);
	new_cmd = malloc(sizeof(char *) * (size + 1));
	if (!new_cmd)
		return (free_tab(cmd1), free_tab(cmd2), NULL);
	idx = -1;
	while (cmd1[++idx])
		new_cmd[idx] = cmd1[idx];
	new_idx = -1;
	while (cmd2[++new_idx])
		new_cmd[idx++] = cmd2[new_idx];
	new_cmd[idx] = NULL;
	free_tab(cmd1);
	free_tab(cmd2);
	return (new_cmd);
}
