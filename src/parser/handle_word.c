/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 21:09:49 by aerenler          #+#    #+#             */
/*   Updated: 2025/08/26 21:09:49 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static int	needs_boundary(const char *left, const char *right)
{
	if (!left || !right || !*left || !*right)
		return (0);
	if (!(ft_isalnum((unsigned char)right[0]) || right[0] == '_'))
		return (0);
	return (is_valid_suffix_var(left));
}

static char	*join_parts(char **parts, int count, int total_len, int boundaries)
{
	char	*final_word;
	int		pos;
	int		j;
	int		k;

	final_word = malloc(total_len + boundaries + 1);
	if (!final_word)
	{
		while (--count >= 0)
			free(parts[count]);
		return (NULL);
	}
	pos = 0;
	j = -1;
	while (++j < count)
	{
		k = 0;
		while (parts[j][k])
			final_word[pos++] = parts[j][k++];
		if (j + 1 < count && needs_boundary(parts[j], parts[j + 1]))
			final_word[pos++] = '\1';
		free(parts[j]);
	}
	final_word[pos] = '\0';
	return (final_word);
}

void	handle_word(t_token **tokens, char **parts, int count, char quote)
{
	char	*final_word;
	int		total_len;
	int		boundaries;
	int		j;

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
	final_word = join_parts(parts, count, total_len, boundaries);
	if (!final_word)
		return ;
	append_token(tokens, new_token_with_quotes(final_word, WORD, quote));
}
