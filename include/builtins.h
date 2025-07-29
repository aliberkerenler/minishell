/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ualkan <ualkan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:58:20 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/28 14:58:20 by ualkan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int		builtin_echo(t_command *cmd);
int		builtin_pwd(void);
int		builtin_cd(t_command *cmd, t_shell *shell);
int		builtin_env(t_shell *shell);
int		builtin_export(t_command *cmd, t_shell *shell);
int		builtin_unset(t_command *cmd, t_shell *shell);
int		builtin_exit(t_command *cmd, t_shell *shell);
void	print_sorted_env(t_shell *shell);
void	free_shell_resources(t_shell *shell);

#endif