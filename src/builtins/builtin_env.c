/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:14:53 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 14:14:53 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	builtin_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->envp && shell->envp[i])
	{
		if (ft_strchr(shell->envp[i], '='))
			printf("%s\n", shell->envp[i]);
		i++;
	}
	return (0);
}
