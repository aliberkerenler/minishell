/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assignment_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:00:36 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 15:02:30 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

int	handle_assignment_with_quotes(const char *input, int start,
		t_token **tokens)
{
	int		i;
	char	quote_char;
	char	*assignment;

	i = start;
	while (input[i] && input[i] != '=')
		i++;
	i++;
	quote_char = input[i];
	i++;
	while (input[i] && input[i] != quote_char)
		i++;
	if (input[i] == quote_char)
		i++;
	assignment = ft_substr(input, start, i - start);
	append_token(tokens, new_token(assignment, WORD));
	return (i);
}

int	is_assignment_with_quotes(const char *input, int start)
{
	int	i;

	i = start;
	if (!input[i] || (!ft_isalpha(input[i]) && input[i] != '_'))
		return (0);
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	if (!input[i] || input[i] != '=')
		return (0);
	i++;
	if (!input[i] || (input[i] != '"' && input[i] != '\''))
		return (0);
	return (1);
}
