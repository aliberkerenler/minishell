/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_expand.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:58:00 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/28 16:58:00 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static size_t	calculate_expanded_len(const char *str, t_shell *shell)
{
	size_t	len;
	int		i;

	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
			len += process_variable_length(str, &i, shell);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

static int	process_variable_expansion(const char *str, char *result,
		int *counters, t_shell *shell)
{
	char	*name;
	char	*value;
	int		len;

	len = get_var_name_len(str + counters[0] + 1);
	if (len <= 0)
		return (0);
	name = ft_substr(str, counters[0] + 1, len);
	if (!name)
		return (0);
	value = get_env_value(name, shell);
	free(name);
	if (!value)
		return (0);
	append_var(result, &counters[1], value);
	free(value);
	counters[0] += len + 1;
	return (1);
}

char	*expand_variables(const char *str, t_shell *shell)
{
	char	*result;
	int		counters[2];

	result = (char *)malloc(calculate_expanded_len(str, shell) + 1);
	if (!result)
		return (NULL);
	counters[0] = 0;
	counters[1] = 0;
	while (str[counters[0]])
	{
		if (str[counters[0]] == '$' && str[counters[0] + 1])
		{
			if (!process_variable_expansion(str, result, counters, shell))
				result[counters[1]++] = str[counters[0]++];
		}
		else
			result[counters[1]++] = str[counters[0]++];
	}
	result[counters[1]] = '\0';
	return (result);
}

static void	process_argument_expansion(char *arg, char **new_args,
										int *new_count, t_shell *shell)
{
	char	*expanded;

	if (ft_strchr(arg, '$'))
	{
		expanded = expand_variables(arg, shell);
		if (expanded && ft_strlen(expanded) > 0)
		{
			new_args[*new_count] = expanded;
			(*new_count)++;
		}
		else if (expanded)
			free(expanded);
	}
	else
	{
		new_args[*new_count] = ft_strdup(arg);
		(*new_count)++;
	}
}

void	expand_command_args(t_command *cmd, t_shell *shell)
{
	int		i;
	int		arg_count;
	char	**new_args;
	int		new_count;

	arg_count = count_args(cmd->args);
	new_args = malloc(sizeof(char *) * (arg_count + 1));
	if (!new_args)
		return ;
	new_count = 0;
	i = 0;
	while (cmd->args[i])
	{
		process_argument_expansion(cmd->args[i], new_args, &new_count, shell);
		i++;
	}
	new_args[new_count] = NULL;
	free_old_args(cmd->args);
	cmd->args = new_args;
}
