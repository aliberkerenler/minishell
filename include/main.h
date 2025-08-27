/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:59:50 by ualkan            #+#    #+#             */
/*   Updated: 2025/07/29 15:18:36 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H
# define LOOP_CONTINUE 0
# define LOOP_BREAK    1
# define LOOP_PROCESS  2

# include "libft.h"
# include "structs.h"
# include "minishell.h"
# include "parser.h"
# include "executor.h"
# include "env.h"
# include "builtins.h"
# include "redirections.h"
# include "signals.h"
# include "readline/readline.h"
# include "readline/history.h"
# include <unistd.h>
# include <signal.h>
# include <errno.h>
# include <sys/wait.h>
# include <fcntl.h>

#endif