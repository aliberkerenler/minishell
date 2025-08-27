/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:00:00 by ualkan            #+#    #+#             */
/*   Updated: 2024/12/19 10:00:00 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>

typedef enum e_shell_state
{
	SHELL_INTERACTIVE,
	SHELL_EXECUTING,
	SHELL_HEREDOC
}	t_shell_state;

extern volatile sig_atomic_t	g_signal_status;

void	init_interactive_signals(void);
void	init_execution_signals(void);
void	init_heredoc_signals(void);
void	handle_signal_in_main_loop(t_shell *shell, t_shell_state state);
void	setup_child_signals(void);
void	heredoc_sigint_handler(int signum);
int		check_and_handle_eof(char *input);

#endif
