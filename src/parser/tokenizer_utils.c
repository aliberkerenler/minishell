/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:50:19 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 14:50:19 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->quote_type = 0;
	token->next = NULL;
	return (token);
}

t_token	*new_token_with_quotes(char *val, t_token_type type, char quote)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = val;
	token->type = type;
	token->quote_type = quote;
	token->next = NULL;
	return (token);
}

void	append_token(t_token **list, t_token *new)
{
	t_token	*current;

	if (!*list)
	{
		*list = new;
		return ;
	}
	current = *list;
	while (current->next)
		current = current->next;
	current->next = new;
}

void	free_token_list(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}

int	is_valid_suffix_var(const char *str)
{
	int	i;

	i = ft_strlen(str) - 1;
	while (i >= 0 && str[i] != '$')
		i--;
	if (i < 0)
		return (0);
	i++;
	if (!str[i])
		return (0);
	if (!(ft_isalpha((unsigned char)str[i]) || str[i] == '_'
			|| str[i] == '?' || str[i] == '$'))
		return (0);
	if (str[i] == '?' || str[i] == '$')
		return (1);
	while (str[i])
	{
		if (!(ft_isalnum((unsigned char)str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}
