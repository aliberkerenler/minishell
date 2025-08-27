/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:00:00 by ualkan            #+#    #+#             */
/*   Updated: 2024/12/19 10:00:00 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	save_std_fds(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		if (*saved_stdin != -1)
			close(*saved_stdin);
		if (*saved_stdout != -1)
			close(*saved_stdout);
		return (-1);
	}
	return (0);
}

static int	restore_stdin_stdout(int saved_stdin, int saved_stdout)
{
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		return (-1);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		return (-1);
	return (0);
}

int	restore_std_fds(int saved_stdin, int saved_stdout)
{
	if (restore_stdin_stdout(saved_stdin, saved_stdout) == -1)
	{
		close(saved_stdin);
		close(saved_stdout);
		return (-1);
	}
	close(saved_stdin);
	close(saved_stdout);
	return (0);
}

static int	process_redir_type(t_redir *current)
{
	if (current->type == REDIR_IN)
		return (handle_redir_in(current));
	else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		return (handle_redir_out(current));
	return (0);
}

int	setup_redirections(t_command *cmd, t_shell *shell)
{
	t_redir	*current;

	if (!cmd)
		return (0);
	if (setup_heredoc(cmd, shell) == -1)
		return (-1);
	current = cmd->redirs;
	while (current)
	{
		if (process_redir_type(current) == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}
