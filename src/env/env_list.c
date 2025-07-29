/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:26:05 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 14:26:05 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	find_env_index(const char *key, t_shell *shell)
{
	int		i;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (shell->envp[i])
	{
		if (ft_strncmp(shell->envp[i], key, key_len) == 0
			&& (shell->envp[i][key_len] == '='
			|| shell->envp[i][key_len] == '\0'))
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

void	update_env(const char *key, const char *value, t_shell *shell)
{
	int		index;
	char	*new_var;
	char	*temp;

	index = find_env_index(key, shell);
	if (value)
	{
		temp = ft_strjoin(key, "=");
		new_var = ft_strjoin(temp, value);
		free(temp);
	}
	else
		new_var = ft_strdup(key);
	if (index != -1)
	{
		free(shell->envp[index]);
		shell->envp[index] = new_var;
	}
	else
	{
		add_env(new_var, shell);
		free(new_var);
	}
}

void	add_env(const char *new_var, t_shell *shell)
{
	int		size;
	int		i;
	char	**new_envp;

	size = 0;
	i = 0;
	while (shell->envp[size])
		size++;
	new_envp = (char **)malloc(sizeof(char *) * (size + 2));
	if (!new_envp)
		return ;
	while (i < size)
	{
		new_envp[i] = shell->envp[i];
		i++;
	}
	new_envp[size] = ft_strdup(new_var);
	new_envp[size + 1] = NULL;
	free(shell->envp);
	shell->envp = new_envp;
}

void	remove_env(const char *key, t_shell *shell)
{
	int		index;
	int		i;

	index = find_env_index(key, shell);
	if (index == -1)
		return ;
	free(shell->envp[index]);
	i = index;
	while (shell->envp[i + 1])
	{
		shell->envp[i] = shell->envp[i + 1];
		i++;
	}
	shell->envp[i] = NULL;
}
