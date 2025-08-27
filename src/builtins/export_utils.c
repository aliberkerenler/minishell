/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:11:22 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 14:24:34 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static void	sort_env_array(char **env_array, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = -1;
	while (++i < count - 1)
	{
		j = -1;
		while (++j < count - i - 1)
		{
			if (ft_strcmp(env_array[j], env_array[j + 1]) > 0)
			{
				temp = env_array[j];
				env_array[j] = env_array[j + 1];
				env_array[j + 1] = temp;
			}
		}
	}
}

static void	print_export_line(const char *env_var)
{
	char	*eq_ptr;
	int		key_len;

	eq_ptr = ft_strchr(env_var, '=');
	if (eq_ptr)
	{
		key_len = eq_ptr - env_var;
		write(1, "declare -x ", 11);
		write(1, env_var, key_len);
		write(1, "=\"", 2);
		write(1, eq_ptr + 1, ft_strlen(eq_ptr + 1));
		write(1, "\"\n", 2);
	}
	else
	{
		write(1, "declare -x ", 11);
		write(1, env_var, ft_strlen(env_var));
		write(1, "\n", 1);
	}
}

void	print_sorted_env(t_shell *shell)
{
	char	**temp_env;
	int		count;
	int		i;

	count = 0;
	while (shell->envp[count])
		count++;
	temp_env = (char **)malloc(sizeof(char *) * (count + 1));
	if (!temp_env)
		return ;
	i = -1;
	while (++i < count)
		temp_env[i] = shell->envp[i];
	temp_env[count] = NULL;
	sort_env_array(temp_env, count);
	i = -1;
	while (++i < count)
		print_export_line(temp_env[i]);
	free(temp_env);
}
