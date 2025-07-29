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

int	read_heredoc_content(const char *delimiter, int fd)
{
	char	*line;
	int		delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	init_heredoc_signals();
	while (1)
	{
		line = readline("> ");
		if (g_signal_status == SIGINT)
		{
			if (line)
				free(line);
			restore_interactive_signals();
			return (-1);
		}
		if (!line)
		{
			write(STDERR_FILENO, "minishell: warning: ", 20);
			write(STDERR_FILENO, "here-document delimited by EOF\n", 32);
			break ;
		}
		if (check_delimiter(line, delimiter, delimiter_len))
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	restore_interactive_signals();
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
	if (open_and_write_heredoc(tempfile, redir) == -1)
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
	return (open_and_redirect_heredoc(tempfile));
}

int	setup_heredoc(t_command *cmd, t_shell *shell)
{
	t_redir	*current;

	if (!cmd)
		return (0);
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
	return (0);
}
