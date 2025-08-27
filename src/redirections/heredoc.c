/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:00:00 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/29 10:08:09 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/main.h"

char	*expand_heredoc_line(char *line, t_shell *shell)
{
	char	*expanded;

	if (!line)
		return (NULL);
	if (!ft_strchr(line, '$'))
		return (line);
	expanded = expand_variables_with_quotes(line, shell, 0);
	if (!expanded)
		return (line);
	free(line);
	return (expanded);
}

int	read_heredoc_content(t_redir *redir, int fd, t_shell *shell)
{
	int		ret;

	if (!redir || !redir->file)
		return (-1);
	init_heredoc_signals();
	while (1)
	{
		ret = read_content_loop(redir, fd, shell);
		if (ret == 0)
			break ;
	}
	init_interactive_signals();
	return (0);
}

int	handle_heredoc(t_redir *redir, t_shell *shell)
{
	char	*tempfile;

	if (!redir || !redir->file || !shell)
		return (-1);
	tempfile = get_heredoc_tempfile();
	if (!tempfile)
		return (-1);
	if (open_and_write_heredoc(tempfile, redir, shell) == -1)
	{
		unlink(tempfile);
		free(tempfile);
		return (-1);
	}
	if (add_tempfile_to_shell(shell, tempfile) == -1)
	{
		unlink(tempfile);
		free(tempfile);
		return (-1);
	}
	return (0);
}

static int	redirect_last_heredoc(t_command *cmd, t_shell *shell)
{
	t_redir	*current;
	t_redir	*last_heredoc;

	if (!cmd || !shell)
		return (0);
	last_heredoc = NULL;
	current = cmd->redirs;
	while (current)
	{
		if (current->type == HEREDOC)
			last_heredoc = current;
		current = current->next;
	}
	if (last_heredoc && shell->tempfile_count > 0)
	{
		return (open_and_redirect_heredoc(
				shell->heredoc_tempfiles[shell->tempfile_count - 1]));
	}
	return (0);
}

int	setup_heredoc(t_command *cmd, t_shell *shell)
{
	t_redir	*current;

	current = cmd->redirs;
	while (current)
	{
		if (current->type == HEREDOC)
		{
			if (handle_heredoc(current, shell) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (redirect_last_heredoc(cmd, shell));
}
