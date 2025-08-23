/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 15:28:25 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/29 15:09:29 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

typedef struct s_shell
{
	char	**envp;
	int		last_exit_status;
	char	*pwd;
	char	*old_pwd;
	char	**heredoc_tempfiles;
	int		tempfile_count;
}	t_shell;

#endif