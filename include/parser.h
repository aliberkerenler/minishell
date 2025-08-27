/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:28:30 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/28 15:28:31 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

t_token		*tokenize(const char *input);
void		free_token_list(t_token *tokens);
int			build_word_token(const char *input, int start, t_token **tokens);
t_token		*new_token(char *value, t_token_type type);
t_token		*new_token_with_quotes(char *val, t_token_type type, char quote);
void		append_token(t_token **list, t_token *new);
int			is_valid_suffix_var(const char *str);
void		handle_word(t_token **tokens, char **parts, int count, char quote);

t_command	*parse(t_token *tokens);
int			check_syntax(t_token *tokens);
void		free_command_list(t_command *cmds);

t_command	*create_cmd_node(void);
int			count_arg(t_token *start, t_token *end);
void		fill_args(t_command *cmd, t_token **start, t_token *end);
void		append_command(t_command **list, t_command *new_cmd);

#endif