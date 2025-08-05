/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:10:00 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/28 21:55:12 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static char	*search_in_path(const char *cmd, char **paths)
{
	char	*path;
	char	*full_path;
	int		i;

	i = -1;
	while (paths && paths[++i])
	{
		path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path, cmd);
		free(path);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
	}
	return (NULL);
}

char	*get_command_path(const char *cmd)
{
	char	**paths;
	char	*path_env;
	char	*result;

	if (!cmd || cmd[0] == '\0')
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	result = search_in_path(cmd, paths);
	if (paths)
		ft_free_split(paths);
	return (result);
}

void	handle_execution_error(const char *cmd_name)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd((char *)cmd_name, 2);
	ft_putstr_fd(": command not found\n", 2);
}

int	is_builtin(char *cmd)
{
	const char	*builtins[] = {"echo", "cd", "pwd", "export",
		"unset", "env", "exit", NULL};
	int			i;

	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(cmd, builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

void	setup_pipes(t_exec_context *ctx, int is_last_cmd)
{
	if (ctx->prev_pipe_read != -1 && !is_last_cmd)
	{
		close(ctx->prev_pipe_read);
		ctx->prev_pipe_read = -1;
	}
}
