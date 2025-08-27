/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:15:09 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 14:15:09 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static int	is_numeric(const char *s)
{
	if (!s || !*s)
		return (0);
	while (*s && (*s == ' ' || *s == '\t'))
		s++;
	if (*s == '-' || *s == '+')
		s++;
	if (!*s || !ft_isdigit(*s))
		return (0);
	while (*s)
	{
		if (!ft_isdigit(*s))
		{
			while (*s && (*s == ' ' || *s == '\t'))
				s++;
			return (*s == '\0');
		}
		s++;
	}
	return (1);
}

static void	perform_exit(t_command *cmd, t_shell *shell, int status)
{
	free_command_list(cmd);
	free_shell_resources(shell);
	exit(status);
}

static void	handle_exit_non_numeric(t_command *cmd, t_shell *shell)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(cmd->args[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	perform_exit(cmd, shell, 2);
}

static int	handle_exit_too_many_args(t_shell *shell)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	shell->last_exit_status = 1;
	return (1);
}

int	builtin_exit(t_command *cmd, t_shell *shell)
{
	int	status;

	write(1, "exit\n", 5);
	if (!cmd->args[1])
	{
		status = shell->last_exit_status;
		perform_exit(cmd, shell, status);
	}
	if (!is_numeric(cmd->args[1]))
		handle_exit_non_numeric(cmd, shell);
	if (cmd->args[2])
		return (handle_exit_too_many_args(shell));
	status = ft_atoi(cmd->args[1]);
	status = ((status % 256) + 256) % 256;
	perform_exit(cmd, shell, status);
	return (0);
}
