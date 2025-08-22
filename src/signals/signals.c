/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:00:00 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/29 10:05:32 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/main.h"

volatile sig_atomic_t	g_signal_status = 0;

static void	interactive_sigint_handler(int signum)
{
	g_signal_status = signum;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	execution_signal_handler(int signum)
{
	g_signal_status = signum;
}

void	heredoc_sigint_handler(int signum)
{
	g_signal_status = signum;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void	init_interactive_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = interactive_sigint_handler;
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
}

void	init_execution_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = execution_signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
