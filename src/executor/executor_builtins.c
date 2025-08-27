/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:10:00 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/28 16:10:00 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	execute_parent_builtin(t_command *cmd, t_shell *shell)
{
	int	status;

	status = 0;
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		status = builtin_cd(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "export") == 0 && cmd->args[1])
		status = builtin_export(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "unset") == 0)
		status = builtin_unset(cmd, shell);
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		status = builtin_exit(cmd, shell);
	else
		return (-1);
	return (status);
}

void	execute_child_builtin(t_command *cmd, t_shell *shell)
{
	int	status;

	status = 0;
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		status = builtin_echo(cmd);
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		status = builtin_pwd();
	if (ft_strcmp(cmd->args[0], "env") == 0)
		status = builtin_env(shell);
	if (ft_strcmp(cmd->args[0], "export") == 0)
		status = builtin_export(cmd, shell);
	free_command_list(cmd);
	free_shell_resources(shell);
	exit(status);
}
