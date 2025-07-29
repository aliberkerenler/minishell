/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:38:16 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 14:38:16 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_command_list(t_command *cmds)
{
	t_command	*cmd_tmp;
	t_redir		*redir_tmp;

	while (cmds)
	{
		cmd_tmp = cmds->next_command;
		free_args(cmds->args);
		free(cmds->quote_types);
		while (cmds->redirs)
		{
			redir_tmp = cmds->redirs->next;
			free(cmds->redirs->file);
			free(cmds->redirs);
			cmds->redirs = redir_tmp;
		}
		free(cmds);
		cmds = cmd_tmp;
	}
}
