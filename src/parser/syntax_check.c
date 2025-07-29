/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:48:00 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 14:48:00 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static int	check_initial_pipe(t_token *tokens)
{
	if (tokens && tokens->type == PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (0);
	}
	return (1);
}

static int	check_redirection_syntax(t_token *token)
{
	if (!token->next)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
		return (0);
	}
	if (token->type != PIPE && token->next->type != WORD)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(token->value, 2);
		ft_putstr_fd("'\n", 2);
		return (0);
	}
	return (1);
}

static int	check_pipe_syntax(t_token *token)
{
	if (!token->next)
	{
		ft_putstr_fd("minishell:", 2);
		ft_putstr_fd(" syntax error near unexpected token `newline'\n", 2);
		return (0);
	}
	if (token->type == PIPE && token->next->type != WORD
		&& token->next->type < REDIR_IN)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putstr_fd(token->next->value, 2);
		ft_putstr_fd("'\n", 2);
		return (0);
	}
	return (1);
}

static int	check_consecutive_pipes(t_token *token)
{
	if (token->type == PIPE && token->next && token->next->type == PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		return (0);
	}
	return (1);
}

int	check_syntax(t_token *tokens)
{
	if (!check_initial_pipe(tokens))
		return (0);
	while (tokens)
	{
		if (tokens->type >= PIPE && tokens->type <= HEREDOC)
		{
			if (tokens->type == PIPE)
			{
				if (!check_pipe_syntax(tokens))
					return (0);
			}
			else
			{
				if (!check_redirection_syntax(tokens))
					return (0);
			}
		}
		if (!check_consecutive_pipes(tokens))
			return (0);
		tokens = tokens->next;
	}
	return (1);
}
