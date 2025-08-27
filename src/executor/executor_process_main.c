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

static int	execute_single_builtin(t_command *cmd, t_shell *shell)
{
	int	status;
	int	saved_stdin;
	int	saved_stdout;

	if (expand_command_args(cmd, shell) == -1)
		return (1);
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		return (0);
	if (save_std_fds(&saved_stdin, &saved_stdout) == -1)
		return (1);
	if (setup_redirections(cmd, shell) != 0)
	{
		shell->last_exit_status = 1;
		restore_std_fds(saved_stdin, saved_stdout);
		return (1);
	}
	status = execute_parent_builtin(cmd, shell);
	restore_std_fds(saved_stdin, saved_stdout);
	return (status);
}

static void	read_heredoc_to_stdout(t_shell *shell)
{
	int		fd;
	char	buf[4096];
	ssize_t	r;

	if (shell->tempfile_count <= 0)
		return ;
	fd = open(shell->heredoc_tempfiles[shell->tempfile_count - 1], O_RDONLY);
	if (fd == -1)
		return ;
	while (1)
	{
		r = read(fd, buf, sizeof(buf));
		if (r <= 0)
			break ;
		write(STDOUT_FILENO, buf, r);
	}
	close(fd);
}

static int	handle_redirection_only_command(t_command *cmd, t_shell *shell)
{
	int	saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		return (shell->last_exit_status = 1, 1);
	if (setup_redirections(cmd, shell) != 0)
	{
		close(saved_stdin);
		return (shell->last_exit_status = 1, 1);
	}
	read_heredoc_to_stdout(shell);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	return (shell->last_exit_status);
}

int	execute_commands(t_command *cmd, t_shell *shell)
{
	int	status;

	if (!cmd)
		return (0);
	if ((!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0') && cmd->redirs)
		return (handle_redirection_only_command(cmd, shell));
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
		return (0);
	if (!cmd->next_command && is_builtin(cmd->args[0]))
	{
		status = execute_single_builtin(cmd, shell);
		if (status != -1)
		{
			shell->last_exit_status = status;
			return (status);
		}
	}
	init_execution_signals();
	status = execute_pipeline(cmd, shell);
	init_interactive_signals();
	handle_signal_in_main_loop(shell, SHELL_EXECUTING);
	shell->last_exit_status = status;
	return (status);
}
