/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:39:48 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 14:39:48 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

t_command	*create_cmd_node(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->quote_types = NULL;
	cmd->redirs = NULL;
	cmd->next_command = NULL;
	return (cmd);
}

void	append_command(t_command **list, t_command *new_cmd)
{
	t_command	*current;

	if (!*list)
	{
		*list = new_cmd;
		return ;
	}
	current = *list;
	while (current->next_command)
		current = current->next_command;
	current->next_command = new_cmd;
}

int	count_arg(t_token *start, t_token *end)
{
	int	count;

	count = 0;
	while (start != end)
	{
		if (start->type == WORD)
			count++;
		else if (start->type >= REDIR_IN && start->type <= HEREDOC)
			start = start->next;
		start = start->next;
	}
	return (count);
}

static void	fill_arg_loop(t_command *cmd, t_token **start, t_token *end)
{
	int	i;

	i = 0;
	while (*start != end)
	{
		if ((*start)->type == WORD)
		{
			cmd->args[i] = ft_strdup((*start)->value);
			cmd->quote_types[i] = (*start)->quote_type;
			i++;
		}
		else if ((*start)->type >= REDIR_IN && (*start)->type <= HEREDOC)
			*start = (*start)->next;
		*start = (*start)->next;
	}
	cmd->args[i] = NULL;
	cmd->quote_types[i] = 0;
}

void	fill_args(t_command *cmd, t_token **start, t_token *end)
{
	int	arg_count;

	arg_count = count_arg(*start, end);
	cmd->args = (char **)malloc(sizeof(char *) * (arg_count + 1));
	cmd->quote_types = (char *)malloc(sizeof(char) * (arg_count + 1));
	if (!cmd->args || !cmd->quote_types)
	{
		if (cmd->args)
			free(cmd->args);
		if (cmd->quote_types)
			free(cmd->quote_types);
		cmd->args = NULL;
		cmd->quote_types = NULL;
		return ;
	}
	fill_arg_loop(cmd, start, end);
}
