/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:15:30 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 14:15:30 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static char	*strip_quotes(const char *str)
{
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 && ((str[0] == '"' && str[len - 1] == '"')
			|| (str[0] == '\'' && str[len - 1] == '\'')))
	{
		return (ft_substr(str, 1, len - 2));
	}
	return (ft_strdup(str));
}

static int	process_export_arg(char *arg, t_shell *shell)
{
	char	*eq_ptr;
	char	*clean_value;

	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: '", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (1);
	}
	eq_ptr = ft_strchr(arg, '=');
	if (eq_ptr)
	{
		*eq_ptr = '\0';
		clean_value = strip_quotes(eq_ptr + 1);
		update_env(arg, clean_value, shell);
		if (clean_value)
			free(clean_value);
		*eq_ptr = '=';
	}
	else if (find_env_index(arg, shell) == -1)
		update_env(arg, NULL, shell);
	return (0);
}

int	builtin_export(t_command *cmd, t_shell *shell)
{
	int	i;
	int	ret_status;

	ret_status = 0;
	if (!cmd->args[1])
	{
		print_sorted_env(shell);
		return (0);
	}
	i = 1;
	while (cmd->args[i])
	{
		if (process_export_arg(cmd->args[i], shell) != 0)
			ret_status = 1;
		i++;
	}
	return (ret_status);
}
