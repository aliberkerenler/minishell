/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_words.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:10:07 by aerenler          #+#    #+#             */
/*   Updated: 2025/08/26 21:10:07 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static char	*get_quoted_part(const char *input, int *i, int *error)
{
	int		start;
	char	quote_char;

	quote_char = input[*i];
	start = *i + 1;
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (!input[*i])
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", 2);
		*error = 1;
		return (NULL);
	}
	(*i)++;
	return (ft_substr(input, start, *i - start - 1));
}

static char	*get_unquoted_part(const char *input, int *i)
{
	int	start;

	start = *i;
	while (input[*i]
		&& !ft_strchr(" \t\n\v\f\r|<>'\"", input[*i])
		&& !(input[*i] == '$' && (input[*i + 1] == '"'
				|| input[*i + 1] == '\'')))
	{
		(*i)++;
	}
	return (ft_substr(input, start, *i - start));
}

static void	free_word_parts(char **word_parts, int part_count)
{
	while (part_count > 0)
	{
		(part_count)--;
		if (word_parts[part_count])
			free(word_parts[part_count]);
	}
}

static int	word_loop(const char *input, char **word_parts,
	int *i, int *part_count)
{
	int	error;

	error = 0;
	while (input[*i] && !ft_strchr(" \t\n\v\f\r|<>", input[*i]))
	{
		if (input[*i] == '$' && (input[*i + 1] == '"' || input[*i + 1] == '\''))
		{
			(*i)++;
			word_parts[*part_count] = get_quoted_part(input, i, &error);
		}
		else if (input[*i] == '\'' || input[*i] == '"')
			word_parts[*part_count] = get_quoted_part(input, i, &error);
		else
			word_parts[*part_count] = get_unquoted_part(input, i);
		if (error || !word_parts[*part_count])
		{
			free_word_parts(word_parts, *part_count);
			return (-1);
		}
		(*part_count)++;
	}
	return (0);
}

int	build_word_token(const char *input, int start, t_token **tokens)
{
	char	*word_parts[256];
	int		part_count;
	char	quote_char;
	int		i;

	part_count = 0;
	i = start;
	quote_char = 0;
	if (input[i] == '\'' || input[i] == '"')
		quote_char = input[i];
	if (word_loop(input, word_parts, &i, &part_count) == -1)
		return (-1);
	handle_word(tokens, word_parts, part_count, quote_char);
	return (i);
}
