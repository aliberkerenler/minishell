/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_process_helpers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:58:00 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/28 16:58:00 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static void	handle_execve_error(t_command *cmd, char *cmd_path, t_shell *shell)
{
	if (errno == EACCES)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": Permission denied\n", 2);
		free(cmd_path);
		free_command_list(cmd);
		free_shell_resources(shell);
		_exit(126);
	}
	else if (errno == ENOEXEC)
	{
		free(cmd_path);
		free_command_list(cmd);
		free_shell_resources(shell);
		_exit(0);
	}
	else
	{
		perror(cmd->args[0]);
		free(cmd_path);
		free_command_list(cmd);
		free_shell_resources(shell);
		_exit(126);
	}
}

void	execute_external_command(t_command *cmd, t_shell *shell)
{
	char	*cmd_path;

	cmd_path = get_command_path(cmd->args[0]);
	if (!cmd_path)
	{
		handle_execution_error(cmd->args[0]);
		free_command_list(cmd);
		free_shell_resources(shell);
		exit(127);
	}
	execve(cmd_path, cmd->args, shell->envp);
	handle_execve_error(cmd, cmd_path, shell);
}

void	setup_child_pipes(int in_fd, int *pipe_fd, t_command *cmd)
{
	if (in_fd != STDIN_FILENO)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (cmd->next_command)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
}

void	cleanup_parent_pipes(int in_fd, int *pipe_fd, t_command *cmd)
{
	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (cmd->next_command)
		close(pipe_fd[1]);
}
