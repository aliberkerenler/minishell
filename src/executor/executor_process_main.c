/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_process_main.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:58:00 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/28 16:58:00 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/main.h"
#include "../../include/signals.h"

void	child_process_execution(t_command *cmd, t_shell *shell)
{
	setup_child_signals();
	expand_command_args(cmd, shell);
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		exit(0);
	if (setup_redirections(cmd, shell) != 0)
		exit(1);
	if (is_builtin(cmd->args[0]))
	{
		execute_child_builtin(cmd, shell);
		exit(0);
	}
	execute_external_command(cmd, shell);
}

static int	wait_for_processes(pid_t last_pid)
{
	int	last_status;

	waitpid(last_pid, &last_status, 0);
	while (wait(NULL) != -1)
		;
	if (WIFSIGNALED(last_status) && WTERMSIG(last_status) == SIGINT)
		write(1, "\n", 1);
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	else if (WIFSIGNALED(last_status))
		return (128 + WTERMSIG(last_status));
	return (last_status);
}

int	execute_pipeline(t_command *cmd, t_shell *shell)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;

	in_fd = STDIN_FILENO;
	while (cmd)
	{
		if (cmd->next_command)
			if (pipe(pipe_fd) == -1)
				return (perror("pipe"), 1);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
		{
			setup_child_pipes(in_fd, pipe_fd, cmd);
			child_process_execution(cmd, shell);
		}
		cleanup_parent_pipes(in_fd, pipe_fd, cmd);
		if (cmd->next_command)
			in_fd = pipe_fd[0];
		cmd = cmd->next_command;
	}
	return (wait_for_processes(pid));
}

int	execute_commands(t_command *cmd, t_shell *shell)
{
	int	status;

	if (!cmd || !cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		return (0);
	if (!cmd->next_command && is_builtin(cmd->args[0]))
	{
		int	saved_stdin, saved_stdout;
		
		expand_command_args(cmd, shell);
		if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
			return (0);
		if (save_std_fds(&saved_stdin, &saved_stdout) == -1)
			return (1);
		if (setup_redirections(cmd, shell) != 0)
		{
			/* Ensure $? reflects redirection failure just like bash (exit status 1) */
			shell->last_exit_status = 1;
			restore_std_fds(saved_stdin, saved_stdout);
			close(saved_stdin);
			close(saved_stdout);
			return (1);
		}
		status = execute_parent_builtin(cmd, shell);
		restore_std_fds(saved_stdin, saved_stdout);
		close(saved_stdin);
		close(saved_stdout);
		if (status != -1)
		{
			shell->last_exit_status = status;
			return (status);
		}
	}
	init_execution_signals();
	status = execute_pipeline(cmd, shell);
	restore_signals_after_execution();
	handle_signal_in_main_loop(shell, SHELL_EXECUTING);
	shell->last_exit_status = status;
	return (status);
}
