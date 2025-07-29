/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:11:01 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 14:11:01 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static int	validate_cd_args(t_command *cmd)
{
	if (cmd->args[1] && cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

static char	*get_target_path(t_command *cmd)
{
	char	*path;

	if (cmd->args[1] != NULL)
	{
		path = cmd->args[1];
	}
	else
	{
		path = getenv("HOME");
	}
	if (!path)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (NULL);
	}
	return (path);
}

static void	update_pwd_vars(char *old_pwd, t_shell *shell)
{
	char	current_pwd_buffer[1024];

	free(shell->old_pwd);
	shell->old_pwd = ft_strdup(old_pwd);
	update_env("OLDPWD", shell->old_pwd, shell);
	if (getcwd(current_pwd_buffer, sizeof(current_pwd_buffer)) != NULL)
	{
		free(shell->pwd);
		shell->pwd = ft_strdup(current_pwd_buffer);
		update_env("PWD", shell->pwd, shell);
	}
}

int	builtin_cd(t_command *cmd, t_shell *shell)
{
	char	*path;
	char	current_pwd_buffer[1024];

	if (getcwd(current_pwd_buffer, sizeof(current_pwd_buffer)) == NULL)
	{
		perror("minishell: cd");
		return (1);
	}
	if (validate_cd_args(cmd) != 0)
		return (1);
	path = get_target_path(cmd);
	if (!path)
		return (1);
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	update_pwd_vars(current_pwd_buffer, shell);
	return (0);
}
