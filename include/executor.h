/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:28:17 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/29 15:27:51 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

int		execute_commands(t_command *cmd, t_shell *shell);
int		execute_pipeline(t_command *cmd, t_shell *shell);
void	child_process_execution(t_command *cmd, t_shell *shell);

void	execute_external_command(t_command *cmd, t_shell *shell);
void	setup_child_pipes(int in_fd, int *pipe_fd, t_command *cmd);
void	cleanup_parent_pipes(int in_fd, int *pipe_fd, t_command *cmd);

void	expand_command_args(t_command *cmd, t_shell *shell);
char	*expand_variables(const char *str, t_shell *shell);

char	*get_env_value(const char *var_name, t_shell *shell);
int		get_var_name_len(const char *str);
size_t	process_variable_length(const char *str, int *i, t_shell *shell);
void	append_var(char *result, int *j, char *value);

int		execute_parent_builtin(t_command *cmd, t_shell *shell);
void	execute_child_builtin(t_command *cmd, t_shell *shell);

char	*get_command_path(const char *cmd);
void	handle_execution_error(const char *cmd_name);
int		is_builtin(char *cmd);
void	ft_free_split(char **split_array);

int		count_args(char **args);
void	free_old_args(char **old_args);
int		count_str_array(char **array);

int		ft_strcmp(const char *s1, const char *s2);

#endif