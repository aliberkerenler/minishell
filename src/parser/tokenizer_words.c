/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_words.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:59:51 by aerenler          #+#    #+#             */
/*   Updated: 2025/08/20 17:32:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static void	join_and_free_parts(char *dest, char **parts, int count, int len)
{
	int	j;

	dest[0] = '\0';
	j = -1;
	while (++j < count)
	{
		ft_strlcat(dest, parts[j], len + 1);
		free(parts[j]);
	}
}

static void	finalize_word(t_token **tokens, char **parts, int count, char quote)
{
	char	*final_word;
	int		total_len;
	int		j;

	if (count == 0)
		return ;
	total_len = 0;
	j = -1;
	while (++j < count)
		total_len += ft_strlen(parts[j]);
	final_word = malloc(total_len + 1);
	if (!final_word)
	{
		j = count;
		while (--j >= 0)
			free(parts[j]);
		return ;
	}
	join_and_free_parts(final_word, parts, count, total_len);
	append_token(tokens, new_token_with_quotes(final_word, WORD, quote));
}

static char	*get_quoted_part(const char *input, int *i)
{
	int		start;
	char	quote_char;

	quote_char = input[*i];
	start = *i + 1;
	(*i)++;
	while (input[*i] && input[*i] != quote_char)
		(*i)++;
	if (input[*i])
		(*i)++;
	return (ft_substr(input, start, *i - start - 1));
}

static char	*get_unquoted_part(const char *input, int *i)
{
	int	start;

	start = *i;
	while (input[*i] && !ft_strchr(" \t\n\v\f\r|<>'\"", input[*i]))
		(*i)++;
	return (ft_substr(input, start, *i - start));
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
	while (input[i] && !ft_strchr(" \t\n\v\f\r|<>", input[i]))
	{
		if (input[i] == '\'' || input[i] == '"')
			word_parts[part_count++] = get_quoted_part(input, &i);
		else
			word_parts[part_count++] = get_unquoted_part(input, &i);
	}
	finalize_word(tokens, word_parts, part_count, quote_char);
	return (i);
}
