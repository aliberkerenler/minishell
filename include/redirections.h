/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:58:13 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/29 15:11:06 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

int		handle_redir_in(t_redir *redir);

int		handle_redir_out(t_redir *redir);

int		handle_heredoc(t_redir *redir, t_shell *shell);
int		setup_heredoc(t_command *cmd, t_shell *shell);
int		read_heredoc_content(const char *delimiter, int fd);

char	*get_heredoc_tempfile(void);
int		add_tempfile_to_shell(t_shell *shell, char *tempfile);
int		check_delimiter(char *line, const char *delimiter, int len);
int		open_and_write_heredoc(char *tempfile, t_redir *redir);
int		open_and_redirect_heredoc(char *tempfile);

int		setup_redirections(t_command *cmd, t_shell *shell);
int		restore_std_fds(int saved_stdin, int saved_stdout);
int		save_std_fds(int *saved_stdin, int *saved_stdout);

#endif
