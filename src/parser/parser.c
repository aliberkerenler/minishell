/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:46:34 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 14:47:00 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static void	add_file(t_redir *redir, t_token **current_token)
{
	if ((*current_token))
	{
		redir->file = ft_strdup((*current_token)->value);
		if (redir->type == HEREDOC)
			redir->quote_type = (*current_token)->quote_type;
	}
	else
		redir->file = NULL;
}

static void	append_redir(t_command *cmd, t_token **current_token)
{
	t_redir	*redir;
	t_redir	*current;

	redir = (t_redir *)malloc(sizeof(t_redir));
	if (!redir)
		return ;
	redir->type = (*current_token)->type;
	redir->quote_type = 0;
	*current_token = (*current_token)->next;
	add_file(redir, current_token);
	redir->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		current = cmd->redirs;
		while (current->next)
			current = current->next;
		current->next = redir;
	}
}

static t_command	*parse_commands_and_redirections(t_token *tokens)
{
	t_command	*cmd_list;
	t_command	*current_cmd;
	t_token		*current_token;

	cmd_list = NULL;
	current_token = tokens;
	current_cmd = create_cmd_node();
	append_command(&cmd_list, current_cmd);
	while (current_token)
	{
		if (current_token->type == PIPE)
		{
			current_cmd = create_cmd_node();
			append_command(&cmd_list, current_cmd);
		}
		else if (current_token->type >= REDIR_IN
			&& current_token->type <= HEREDOC)
		{
			append_redir(current_cmd, &current_token);
		}
		current_token = current_token->next;
	}
	return (cmd_list);
}

static void	parse_arguments(t_command *cmd_list, t_token *tokens)
{
	t_command	*current_cmd;
	t_token		*token_start;
	t_token		*token_end;

	current_cmd = cmd_list;
	token_start = tokens;
	while (current_cmd)
	{
		token_end = token_start;
		while (token_end && token_end->type != PIPE)
			token_end = token_end->next;
		fill_args(current_cmd, &token_start, token_end);
		if (token_end)
			token_start = token_end->next;
		current_cmd = current_cmd->next_command;
	}
}

t_command	*parse(t_token *tokens)
{
	t_command	*cmd_list;

	if (!tokens || !check_syntax(tokens))
		return (NULL);
	cmd_list = parse_commands_and_redirections(tokens);
	if (!cmd_list)
		return (NULL);
	parse_arguments(cmd_list, tokens);
	return (cmd_list);
}
