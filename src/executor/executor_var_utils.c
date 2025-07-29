/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_var_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 16:58:00 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/28 16:58:00 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

char	*get_env_value(const char *var_name, t_shell *shell)
{
	int		index;
	char	*eq_ptr;

	if (ft_strcmp(var_name, "?") == 0)
		return (ft_itoa(shell->last_exit_status));
	index = find_env_index(var_name, shell);
	if (index == -1)
		return (ft_strdup(""));
	eq_ptr = ft_strchr(shell->envp[index], '=');
	if (!eq_ptr)
		return (ft_strdup(""));
	return (ft_strdup(eq_ptr + 1));
}

int	get_var_name_len(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '?' || str[i] == '$')
		return (1);
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

size_t	process_variable_length(const char *str, int *i, t_shell *shell)
{
	char	*var_name;
	char	*var_value;
	size_t	var_value_len;
	int		var_len;

	var_len = get_var_name_len(str + *i + 1);
	if (var_len > 0)
	{
		var_name = ft_substr(str, *i + 1, var_len);
		var_value = get_env_value(var_name, shell);
		var_value_len = ft_strlen(var_value);
		*i += var_len + 1;
		free(var_name);
		free(var_value);
		return (var_value_len);
	}
	else
	{
		(*i)++;
		return (1);
	}
}

void	append_var(char *result, int *j, char *value)
{
	if (ft_strlen(value) > 0)
	{
		ft_strlcpy(result + *j, value, ft_strlen(value) + 1);
		*j += ft_strlen(value);
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
	{
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
