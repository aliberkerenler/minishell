/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_expand_args.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 09:24:01 by aerenler          #+#    #+#             */
/*   Updated: 2025/08/27 09:24:01 by aerenler         ###   ########.fr       */
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
		if (str[i] == '\1')
		{
			i++;
			continue ;
		}
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

char	*expand_variables_with_quotes(const char *str, t_shell *shell,
		char quote_type)
{
	char	*result;
	int		counters[2];

	if (quote_type == '\'')
		return (ft_strdup(str));
	result = (char *)malloc(calculate_expanded_len(str, shell) + 1);
	if (!result)
		return (NULL);
	counters[0] = 0;
	counters[1] = 0;
	while (str[counters[0]])
	{
		if (str[counters[0]] == '$' && str[counters[0] + 1] && quote_type
			!= '\'')
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

void	process_argument_expansion(char *arg, char quote_type,
		t_shell *shell, t_expansion_context *context)
{
	char	*expanded;

	if (ft_strchr(arg, '$'))
	{
		expanded = expand_variables_with_quotes(arg, shell, quote_type);
		if (expanded)
		{
			if (*expanded || quote_type != 0)
			{
				context->new_args[*(context->new_count)] = expanded;
				(*(context->new_count))++;
			}
			else
				free(expanded);
		}
	}
	else
	{
		context->new_args[*(context->new_count)] = ft_strdup(arg);
		(*(context->new_count))++;
	}
}

int	expand_command_args(t_command *cmd, t_shell *shell)
{
	int					arg_count;
	t_expansion_context	context;
	char				**new_args;
	int					new_count;

	arg_count = count_args(cmd->args);
	new_args = malloc(sizeof(char *) * (arg_count + 1));
	if (!new_args)
		return (-1);
	new_count = 0;
	context.new_args = new_args;
	context.new_count = &new_count;
	if (expand_loop(cmd, shell, &context))
	{
		free_str_array(new_args);
		return (-1);
	}
	new_args[new_count] = NULL;
	free_str_array(cmd->args);
	cmd->args = new_args;
	return (0);
}
