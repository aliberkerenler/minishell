/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:00:00 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/29 10:08:16 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/main.h"

char	*get_heredoc_tempfile(void)
{
	char		*pid_str;
	char		*time_str;
	char		*temp_name;
	char		*temp_base;
	static int	counter = 0;

	pid_str = ft_itoa(getpid());
	time_str = ft_itoa(counter++);
	temp_base = ft_strjoin("/tmp/.minishell_heredoc_", pid_str);
	temp_name = ft_strjoin(temp_base, "_");
	free(temp_base);
	temp_base = ft_strjoin(temp_name, time_str);
	free(temp_name);
	free(pid_str);
	free(time_str);
	return (temp_base);
}

int	add_tempfile_to_shell(t_shell *shell, char *tempfile)
{
	char	**new_array;
	int		i;

	new_array = malloc(sizeof(char *) * (shell->tempfile_count + 1));
	if (!new_array)
		return (-1);
	i = 0;
	while (i < shell->tempfile_count)
	{
		new_array[i] = shell->heredoc_tempfiles[i];
		i++;
	}
	new_array[shell->tempfile_count] = tempfile;
	if (shell->heredoc_tempfiles)
		free(shell->heredoc_tempfiles);
	shell->heredoc_tempfiles = new_array;
	shell->tempfile_count++;
	return (0);
}

int	open_and_write_heredoc(char *tempfile, t_redir *redir, t_shell *shell)
{
	int	temp_fd;

	temp_fd = open(tempfile, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (temp_fd == -1)
		return (-1);
	if (read_heredoc_content(redir, temp_fd, shell) == -1)
	{
		close(temp_fd);
		return (-1);
	}
	close(temp_fd);
	return (0);
}

int	open_and_redirect_heredoc(char *tempfile)
{
	int	read_fd;

	read_fd = open(tempfile, O_RDONLY);
	if (read_fd == -1)
		return (-1);
	if (dup2(read_fd, STDIN_FILENO) == -1)
	{
		free(tempfile);
		close(read_fd);
		return (-1);
	}
	close(read_fd);
	return (0);
}
