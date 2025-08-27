/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:01:37 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 15:01:37 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static int	handle_operator_token(const char *input, int i, t_token **tokens)
{
	if (input[i] == '>' && input[i + 1] == '>')
	{
		append_token(tokens,
			new_token(ft_substr(&input[i], 0, 2), REDIR_APPEND));
		return (i + 2);
	}
	else if (input[i] == '<' && input[i + 1] == '<')
	{
		append_token(tokens, new_token(ft_substr(&input[i], 0, 2), HEREDOC));
		return (i + 2);
	}
	else if (input[i] == '>')
		append_token(tokens, new_token(ft_substr(&input[i], 0, 1), REDIR_OUT));
	else if (input[i] == '<')
		append_token(tokens, new_token(ft_substr(&input[i], 0, 1), REDIR_IN));
	else if (input[i] == '|')
		append_token(tokens, new_token(ft_substr(&input[i], 0, 1), PIPE));
	return (i + 1);
}

t_token	*tokenize(const char *input)
{
	t_token	*tokens;
	int		i;
	int		ret;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (ft_strchr(" \t\n\v\f\r", input[i]))
			i++;
		else if (ft_strchr("|<>", input[i]))
			i = handle_operator_token(input, i, &tokens);
		else
		{
			ret = build_word_token(input, i, &tokens);
			if (ret == -1)
			{
				free_token_list(tokens);
				return (NULL);
			}
			i = ret;
		}
	}
	return (tokens);
}
