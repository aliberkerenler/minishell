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

/* Sentinel char to mark an internal boundary so variable names don't overrun */
#define VAR_BOUNDARY '\1'

static int	needs_boundary(const char *left, const char *right)
{
	int i;

	if (!left || !right || !*left || !*right)
		return (0);
	/* right begins with alnum or underscore? */
	if (!(ft_isalnum((unsigned char)right[0]) || right[0] == '_'))
		return (0);
	/* Scan left for last '$' */
	i = ft_strlen(left) - 1;
	while (i >= 0 && left[i] != '$')
		i--;
	if (i < 0)
		return (0);
	i++; /* first char of potential var name */
	if (!left[i])
		return (0);
	if (!(ft_isalpha((unsigned char)left[i]) || left[i] == '_' || left[i] == '?' || left[i] == '$'))
		return (0);
	/* Special one-char vars */
	if (left[i] == '?' || left[i] == '$')
		return (1);
	/* Ensure remainder to end are valid name chars */
	while (left[i])
	{
		if (!(ft_isalnum((unsigned char)left[i]) || left[i] == '_'))
			return (0); /* something else ends name already */
		i++;
	}
	/* Left ended exactly at end of variable name and right starts with name char -> boundary needed */
	return (1);
}

static void	finalize_word(t_token **tokens, char **parts, int count, char quote)
{
	char	*final_word;
	int		total_len;
	int		j;
	int		boundaries;
	int		pos;

	if (count == 0)
		return ;
	total_len = 0;
	boundaries = 0;
	j = -1;
	while (++j < count)
	{
		total_len += ft_strlen(parts[j]);
		if (j + 1 < count && needs_boundary(parts[j], parts[j + 1]))
			boundaries++;
	}
	final_word = malloc(total_len + boundaries + 1);
	if (!final_word)
	{
		j = count;
		while (--j >= 0)
			free(parts[j]);
		return ;
	}
	pos = 0;
	j = -1;
	while (++j < count)
	{
		int k = 0;
		while (parts[j][k])
			final_word[pos++] = parts[j][k++];
		if (j + 1 < count && needs_boundary(parts[j], parts[j + 1]))
			final_word[pos++] = VAR_BOUNDARY;
		free(parts[j]);
	}
	final_word[pos] = '\0';
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
