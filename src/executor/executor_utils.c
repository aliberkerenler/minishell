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
		if (access(full_path, F_OK) == 0)
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
		if (access(cmd, F_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	result = search_in_path(cmd, paths);
	if (paths)
		free_str_array(paths);
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

int	expand_loop(t_command *cmd, t_shell *shell,
			t_expansion_context *context)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		process_argument_expansion(cmd->args[i], cmd->quote_types[i],
			shell, context);
		if (!context->new_args)
			return (1);
		i++;
	}
	return (0);
}
