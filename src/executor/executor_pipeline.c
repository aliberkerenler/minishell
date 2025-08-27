/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aerenler <aerenler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 10:48:33 by aerenler          #+#    #+#             */
/*   Updated: 2025/08/27 10:50:06 by aerenler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.h"

static void	child_process_execution(t_command *cmd, t_shell *shell)
{
	setup_child_signals();
	if (expand_command_args(cmd, shell) == -1)
	{
		free_command_list(cmd);
		free_shell_resources(shell);
		exit(1);
	}
	if (setup_redirections(cmd, shell) != 0)
	{
		free_command_list(cmd);
		free_shell_resources(shell);
		exit(1);
	}
	if (!cmd->args || !cmd->args[0] || cmd->args[0][0] == '\0')
	{
		free_command_list(cmd);
		free_shell_resources(shell);
		exit(0);
	}
	if (is_builtin(cmd->args[0]))
	{
		execute_child_builtin(cmd, shell);
		exit(0);
	}
	execute_external_command(cmd, shell);
}

static int	wait_for_processes(pid_t last_pid)
{
	int	last_status;

	waitpid(last_pid, &last_status, 0);
	while (wait(NULL) != -1)
		;
	if (WIFSIGNALED(last_status) && WTERMSIG(last_status) == SIGINT)
		write(1, "\n", 1);
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	else if (WIFSIGNALED(last_status))
		return (128 + WTERMSIG(last_status));
	return (last_status);
}

static void	execute_child_process(t_command *cmd, t_pipeline_data *pdata,
		t_shell *shell)
{
	t_command	*tmp;
	t_command	*next;

	if (pdata->head != cmd)
	{
		tmp = pdata->head;
		while (tmp && tmp != cmd)
		{
			next = tmp->next_command;
			tmp->next_command = NULL;
			free_command_list(tmp);
			tmp = next;
		}
	}
	setup_child_pipes(pdata->in_fd, pdata->pipe_fd, cmd);
	child_process_execution(cmd, shell);
}

int	execute_pipeline(t_command *cmd, t_shell *shell)
{
	t_pipeline_data	pdata;
	pid_t			pid;

	pdata.in_fd = STDIN_FILENO;
	pdata.head = cmd;
	pid = 0;
	while (cmd)
	{
		if (cmd->next_command)
			if (pipe(pdata.pipe_fd) == -1)
				return (perror("pipe"), 1);
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
			execute_child_process(cmd, &pdata, shell);
		cleanup_parent_pipes(pdata.in_fd, pdata.pipe_fd, cmd);
		if (cmd->next_command)
			pdata.in_fd = pdata.pipe_fd[0];
		cmd = cmd->next_command;
	}
	return (wait_for_processes(pid));
}
