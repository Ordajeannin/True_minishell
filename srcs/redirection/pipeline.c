/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 17:32:16 by pkorsako          #+#    #+#             */
/*   Updated: 2023/12/22 20:32:33 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_fork_err(int i, int type)
{
	if (i == -1 && type == 2)
	{
		perror("pipe");
		ft_readline(NULL, NO);
		ft_malloc(0, FREE);
		exit(EXIT_FAILURE);
	}
	if (i == -1 && type == 1)
	{
		perror("pipe");
		ft_readline(NULL, NO);
		ft_malloc(0, FREE);
		exit(EXIT_FAILURE);
	}
}

void	child_process(int *pipe_fds, int *prev_pipe, t_args_list *current,
			t_shell *shell)
{
	close(pipe_fds[0]);
	if (prev_pipe[1] != -1)
		close(prev_pipe[1]);
	if (prev_pipe[0] != -1)
	{
		dup2(prev_pipe[0], STDIN_FILENO);
		close(prev_pipe[0]);
	}
	if (current->next != NULL)
		dup2(pipe_fds[1], STDOUT_FILENO);
	args_handle(current->head, shell, &(shell->env_list));
	ft_malloc(0, FREE);
	exit(set_error_nb(0, NO));
}

void	dad_process(int *pipe_fds, int *prev_pipe)
{
	close(pipe_fds[1]);
	if (prev_pipe[0] != -1)
		close(prev_pipe[0]);
	prev_pipe[0] = pipe_fds[0];
}

void	wait_child(void)
{
	int	status;

	while (wait(&status) > 0)
	{
		signal(SIGQUIT, SIG_IGN);
		if (WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0)
			set_error_nb(WEXITSTATUS(status), YES);
	}
}

/*
 * Permet d'executer les commandes en pipeline
 * sans connaitre a l'avance le nombre de sous-chaines
 * (sa norme constitue mon plus grand cauchemard)
 *
*/
void	execute_command(t_args_list **stock, t_shell *shell)
{
	t_args_list	*current;
	int			prev_pipe[2];
	int			pipe_fds[2];
	pid_t		pid;

	current = *stock;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	while (current != NULL)
	{
		pipe_fork_err(pipe(pipe_fds), 2);
		pid = fork();
		pipe_fork_err(pid, 1);
		if (pid == 0)
			child_process(pipe_fds, prev_pipe, current, shell);
		else
			dad_process(pipe_fds, prev_pipe);
		current = current->next;
	}
	wait_child();
}
