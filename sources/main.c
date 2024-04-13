/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:30:34 by lpupier           #+#    #+#             */
/*   Updated: 2023/04/05 14:05:15 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Header
#include "../includes/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	char	*prompt;
	t_list	*new_envp;
	t_env	*env;

	if (!envp[0])
		return (EXIT_FAILURE);
	prompt = init_minishell(argc, argv, envp);
	new_envp = envp_to_list(envp);
	if (!new_envp)
		return (free(prompt), EXIT_FAILURE);
	env = malloc(sizeof(t_env));
	if (!env)
		return (free(prompt), free_list(new_envp), EXIT_FAILURE);
	env->new_envp = new_envp;
	env->fd_in = 0;
	env->fd_out = 1;
	env->og_fd_in = dup(STDIN_FILENO);
	env->og_fd_out = dup(STDOUT_FILENO);
	loop_main(prompt, env);
	free_list(env->new_envp);
	free_tab(env->envp);
	free(env);
	free(prompt);
	return (get_exit_status());
}

char	*init_minishell(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	signal(SIGQUIT, SIG_IGN);
	using_history();
	minishell_header();
	return (ft_strjoin(ft_strjoin(ft_strdup("\001\e[1;94;40m\002"), \
	get_env(envp, "USER")), \
	ft_strdup("@minishell\001\e[0m\002\001\e[1;91;40m\002 \001➜\002\001\e[0m\002 ")));
}

int	loop_main(char *prompt, t_env *env)
{
	char	*command;

	while (1)
	{
		signal(SIGINT, signal_ctrl_c);
		env->envp = list_to_envp(env->new_envp);
		if (!env->envp)
			return (printf("exit\n"), 0);
		command = readline(prompt);
		if (!command)
			return (printf("exit\n"), 0);
		if (command[0] != '\0' && !is_empty(command))
		{
			add_history(command);
			if (!command_interpretation(command, env))
				return (free(command), 0);
		}
		free_tab(env->envp);
		free(command);
	}
	close(env->og_fd_in);
	close(env->og_fd_out);
	return (1);
}

int	command_interpretation(char *command, t_env *env)
{
	int		pipe;
	int		status;
	char	**cmds_pipe;
	char	***cmds;

	if (command[0] == '|' || command[ft_strlen(command) - 1] == '|')
		return (printf("Syntax error near unexpected token '|'\n"), 1);
	pipe = contains(command, '|');
	cmds_pipe = ft_split(command, '|');
	if (!cmds_pipe)
		return (0);
	cmds = malloc(sizeof(char **) * (get_array_size(cmds_pipe) + 1));
	if (!cmds)
		return (free_tab(cmds_pipe), 1);
	cmds[get_array_size(cmds_pipe)] = NULL;
	status = shell_core(cmds_pipe, cmds, pipe, env);
	return (free_2tab(cmds), free_tab(cmds_pipe), status);
}

int	shell_core(char **cmds_pipe, char ***cmds, int pipe, t_env *env)
{
	int	idx;

	idx = -1;
	while (cmds_pipe[++idx])
	{
		if (is_ws_str(cmds_pipe[idx]))
			return (printf("Syntax error near unexpected token '|'\n"), \
			cmds[0] = NULL, 1);
	}
	idx = -1;
	while (cmds_pipe[++idx])
	{
		cmds[idx] = malloc(sizeof(char *));
		if (!cmds[idx])
			return (0);
		cmds[idx][0] = NULL;
		cmds[idx] = cmd_parsing(cmds[idx], cmds_pipe[idx], env->envp);
		if (cmds[idx][0] == NULL)
			return (ft_putstr_fd("\e[31mCommand '' not found.\e[0m\n", 2), 1);
		if (!pipe && check_functions(cmds[idx], env, 0) == EXIT_FAILURE)
			return (0);
	}
	if (pipe)
		ft_pipe(cmds, env);
	return (1);
}
