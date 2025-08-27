/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:28:40 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/29 15:15:08 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	char			quote_type;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	char			*file;
	char			quote_type;
	t_token_type	type;
	struct s_redir	*next;
}	t_redir;

typedef struct s_command
{
	char				**args;
	char				*quote_types;
	t_redir				*redirs;
	struct s_command	*next_command;
}	t_command;

typedef struct s_expansion_context
{
	char	**new_args;
	int		*new_count;
}	t_expansion_context;

typedef struct s_pipeline_data
{
	t_command	*head;
	int			in_fd;
	int			pipe_fd[2];
}	t_pipeline_data;

#endif