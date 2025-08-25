/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:00:00 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/29 10:04:26 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/main.h"

int	check_and_handle_eof(char *input)
{
	if (input == NULL)
	{
		write(STDERR_FILENO, "exit\n", 5);
		return (1);
	}
	return (0);
}

static void	handle_execution_signal(t_shell *shell)
{
	if (g_signal_status == SIGINT)
		shell->last_exit_status = 130;
	else if (g_signal_status == SIGQUIT)
	{
		write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
		shell->last_exit_status = 131;
	}
}

static void	handle_heredoc_signal(t_shell *shell)
{
	if (g_signal_status == SIGINT)
	{
		shell->last_exit_status = 130;
		write(STDOUT_FILENO, "\n", 1);
	}
}

void	handle_signal_in_main_loop(t_shell *shell, t_shell_state state)
{
	if (g_signal_status == 0)
		return ;
	if (state == SHELL_EXECUTING)
		handle_execution_signal(shell);
	else if (state == SHELL_HEREDOC)
		handle_heredoc_signal(shell);
	else
	{
		if (g_signal_status == SIGINT)
			shell->last_exit_status = 130;
	}
	g_signal_status = 0;
}
