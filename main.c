/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:22:30 by aerenler          #+#    #+#             */
/*   Updated: 2025/07/29 15:22:31 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/main.h"
#include "include/signals.h"

void	free_shell_resources(t_shell *shell)
{
	int	i;

	if (!shell)
		return ;
	free_env(shell->envp);
	free(shell->pwd);
	free(shell->old_pwd);
	if (shell->heredoc_tempfiles)
	{
		i = 0;
		while (i < shell->tempfile_count)
		{
			if (shell->heredoc_tempfiles[i])
			{
				unlink(shell->heredoc_tempfiles[i]);
				free(shell->heredoc_tempfiles[i]);
			}
			i++;
		}
		free(shell->heredoc_tempfiles);
	}
}

static void	init_shell(t_shell *shell, char **envp)
{
	char	cwd[1024];

	shell->envp = copy_env(envp);
	shell->last_exit_status = 0;
	shell->pwd = NULL;
	shell->old_pwd = NULL;
	shell->heredoc_tempfiles = NULL;
	shell->tempfile_count = 0;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		shell->pwd = ft_strdup(cwd);
}

static int	read_and_validate_input(char **input_ptr, t_shell *shell)
{
	*input_ptr = readline("minishell> ");
	if (check_and_handle_eof(*input_ptr))
		return (LOOP_BREAK);
	handle_signal_in_main_loop(shell, SHELL_INTERACTIVE);
	if (g_signal_status == SIGINT)
	{
		if (*input_ptr)
			free(*input_ptr);
		return (LOOP_CONTINUE);
	}
	if ((*input_ptr)[0] == '\0')
	{
		free(*input_ptr);
		return (LOOP_CONTINUE);
	}
	return (LOOP_PROCESS);
}

static void	process_and_execute(char *input, t_shell *shell)
{
	t_token		*tokens;
	t_command	*commands;

	add_history(input);
	tokens = tokenize(input);
	free(input);
	if (!tokens)
		return ;
	commands = parse(tokens);
	if (!commands)
	{
		free_token_list(tokens);
		return ;
	}
	free_token_list(tokens);
	execute_commands(commands, shell);
	free_command_list(commands);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	char	*input;
	int		loop_status;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	init_interactive_signals();
	while (1)
	{
		loop_status = read_and_validate_input(&input, &shell);
		if (loop_status == LOOP_BREAK)
			break ;
		if (loop_status == LOOP_CONTINUE)
			continue ;
		process_and_execute(input, &shell);
	}
	rl_clear_history();
	free_shell_resources(&shell);
	return (shell.last_exit_status);
}
