/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/28 01:46:59 by aerenler          #+#    #+#             */
/*   Updated: 2025/08/28 01:47:44 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/main.h"

int	loop_util(t_redir *redir, int fd, t_shell *shell, char *line)
{
	int		delimiter_len;

	delimiter_len = ft_strlen(redir->file);
	if (ft_strlen(line) == delimiter_len
		&& ft_strncmp(line, redir->file, delimiter_len) == 0)
	{
		free(line);
		return (0);
	}
	if (redir->quote_type == 0)
		line = expand_heredoc_line(line, shell);
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
	return (1);
}

int	read_content_loop(t_redir *redir, int fd, t_shell *shell)
{
	char	*line;
	int		delimiter_len;

	if (!redir || !redir->file)
		return (-1);
	delimiter_len = ft_strlen(redir->file);
	line = readline("> ");
	if (g_signal_status == SIGINT)
	{
		if (line)
			free(line);
		return (-1);
	}
	if (!line)
	{
		write(STDERR_FILENO, "minishell: warning: here-document", 36);
		write(STDERR_FILENO, "delimited by EOF\n", 18);
		return (0);
	}
	if (loop_util(redir, fd, shell, line) == 0)
		return (0);
	return (1);
}
