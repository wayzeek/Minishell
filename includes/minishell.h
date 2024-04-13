/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpupier <lpupier@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 13:31:00 by lpupier           #+#    #+#             */
/*   Updated: 2023/04/05 12:53:03 by lpupier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Define
#ifndef MINISHELL_H
# define MINISHELL_H

// Librairies
# include <stdio.h>
# include <limits.h>
# include <stdlib.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>

// Headers
# include "../libft/libft.h"

// Env structure
typedef struct s_env
{
	char	**envp;
	t_list	*new_envp;
	int		fd_in;
	int		fd_out;
	int		og_fd_in;
	int		og_fd_out;
}	t_env;

// Parsing structure
typedef struct s_parsing
{
	char	**envp;
	int		idx;
	int		idx_init;
	char	c;
}	t_parsing;

// main.c

int		main(int argc, char **argv, char **envp);
char	*init_minishell(int argc, char **argv, char **envp);
int		loop_main(char *prompt, t_env *env);
int		command_interpretation(char *command, t_env *env);
int		shell_core(char **cmds_pipe, char ***cmds, int pipe, t_env *env);

// commands.c

int		check_functions(char **cmd, t_env *envi, int status);
int		check_builtins(char **cmd, t_env *envi, int *exit_status);
int		launch_program(char **cmd, int status, t_env *envi);
int		get_exit_status(void);
void	set_exit_status(int code);

// utils.c

pid_t	binary(const char *program, char **args, char **envp);
int		is_in_envp(char **envp, char *request);
char	*get_env(char **envp, char *request);
char	*get_binary_path(char *cmd, char **envp);

// ft_tab.c

void	free_tab(char **tab);
void	free_2tab(char ***tab);
int		get_array_size(char **envp);
void	free_begin_tab(char **tab, int idx);
char	**add_to_tab(char **tab, char *elt);

// get_variable.c

char	*get_var(char *str, char **envp);
int		replace_var(char **envp, char **cmd, int idx, int new_idx);
char	*get_var_init(char **envp, char **cmd, int idx, int *new_idx);
char	**join_2_tab(char **cmd1, char **cmd2);

// parsing.c

char	**cmd_parsing(char **cmd, char *prompt, char **envp);
void	loop_parsing(char ***cmd, char *str, char **envp, \
					t_parsing *parsing);
int		is_ws(char c);
int		is_ws_str(char *str);
int		is_empty(char *str);

// heredoc.c

int		parse_heredoc(char ***cmd, char *str, t_parsing *parsing);
char	*is_heredoc(char *str);
char	**reorganize_tab_heredoc(char **cmd);
int		nb_loop_heredoc(char **cmd);
void	loop_reorganize_heredoc(char **cmd, char ***new_cmd, int len_tab);

// quotes.c

int		parse_quotes(char ***cmd, char *str, t_parsing *parsing);
char	*removing_quotes(char *str);
char	*str_without_quote(char *str, char *new_str);

// signals.c

void	signal_ctrl_c(int id);
void	signal_ctrl_c_child(int id);
void	signal_ctrl_backslash(int id);

// ft_exit.c
int		ft_exit(char **cmd);

// ft_echo.c

int		echo(char **cmd);

// ft_pwd.c

int		pwd(char **cmd);

// ft_env.c

int		env(char **cmd, t_list *new_envp);
void	sort_envp(t_list *start);
t_list	*envp_to_list(char **envp);

// ft_export.c

int		ft_export(char **cmd, t_list *new_envp);
char	*remove_plus(char *str);

// list_utils.c

t_list	*ft_create_elem(void *data);
int		ft_list_contains(t_list *begin_list, void *data_ref, int search);
void	ft_list_push_back(t_list **begin_list, void *data);
t_list	*ft_list_find(t_list *begin_list, void *data_ref, int search);
void	swap_nodes(t_list *a, t_list *b);
char	**list_to_envp(t_list *list);

// ft_unset.c

int		ft_lstdelone(t_list **head_lst, char *data_ref);
int		ft_unset(char **cmd, t_list *new_envp);
int		get_equal_index(char *export_cmd);

// ft_check_export.c

int		check_export_error(char *cmd);
int		print_export(t_list *new_envp);
int		treat_export(char **cmd, t_list *new_envp, int argc);
int		treat_dollar_sign(t_list *envp, char **cmd_split);
int		treat_special_case(char **cmd, t_list *envp, char **cmd_split, int i);

// ft_export_utils.c

int		count_plus(char *str);
int		treat_empty_value(char **cmd, char *export_cmd, t_list *new_envp, \
							int cmd_i);
void	free_list(t_list *list);
char	*rm_extra_tab(char *str);

// ft_more_error.c

int		check_forbidden_char(char *str);
int		count_equals(char *str);
int		treat_multiple_equals(char *export_cmd, t_list *new_envp);

// ft_export_extanted.c

int		export_other_case(char **cmd, t_list *new_envp, int i);
int		treat_plus_sign(char **cmd, char **cmd_split, \
t_list *new_envp, int i);
int		export_new_value(char **cmd, char **cmd_split, \
t_list *new_envp, int i);
int		change_known_var(char **cmd, t_list *new_envp, int i);
int		replace_dollar_value(t_list *envp, char **cmd_split);

// ft_export_expansion.c

void	print_export_equals(t_list *new_envp);
int		check_prompt_error(char *cmd, char **cmd_split);
int		change_value(t_list *envp, char **cmd_split);
int		treat_different_cases(char **cmd, t_list *new_envp, int i);
int		check_all_cases(char **cmd, char **cmd_split, t_list *new_envp, int i);

// ft_cd.c

int		cd(char **cmd, t_env *envi);

// pipe_utils.c

int		count_pipe(char ***cmd_tab);
int		check_redirections(char **cmd);
int		get_redirections_index(char **cmd);
int		make_redirections(char **cmd);
int		check_infiles(char **cmd);

// ft_pipe.c

int		ft_pipe(char ***cmd_tab, t_env *env);
int		exec_cmd(int *prev_fd, int *next_fd, char **cmd, t_env *env);

// ft_pipe_extended.c

int		check_next_fd(int *next_fd, char **cmd);
int		open_infile(char **cmd, int i);
void	free_redirections(int i, char **cmd);
int		handle_both_infiles(char **cmd, t_env *env, int status, int i);

// infiles_utils.c

int		create_heredoc(char **cmd, int i, int fd[2]);
int		handle_without_pipes(char **cmd, int i, t_env *env);
int		make_all_redirections(char **cmd, int i);
int		put_fds_back(t_env *envi);
int		handle_infiles_and_redirections(char **cmd, t_env *envi);

// decoration.c

void	minishell_header(void);

// redirections_utils.c

int		get_infiles_index(char **cmd);
int		handle_infiles(char **cmd, t_env *env, int status);
int		handle_heredoc(char **cmd, int status, int i);
char	**ignore_infile(char **cmd);
char	**ignore_heredoc(char **cmd);

#endif