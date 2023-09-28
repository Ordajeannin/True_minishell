/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 17:55:51 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/28 17:41:08 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * ensemble de fonctions visant a normer execute_command.
 * pas encore fonctionnel
void	close_pipe(int pipe_fd[2])
{
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
	if (pipe_fd[1] != -1)
		close(pipe_fd[1]);
}

void	redirect_input(int prev_pipe_fd[2])
{
	if (prev_pipe_fd[0] != -1)
	{
		dup2(prev_pipe_fd[0], STDIN_FILENO);
		close(prev_pipe_fd[0]);
	}
}

void	redirect_output(int next_pipe_fd[2])
{
	if (next_pipe_fd[1] != -1)
		dup2(next_pipe_fd[1], STDOUT_FILENO);
}

void	execute_child1(int prev_pipe_fd[2], int next_pipe_fd[2])
{
	close_pipe(prev_pipe_fd);
	redirect_input(prev_pipe_fd);
	redirect_output(next_pipe_fd);
}

void	execute_child2(t_args_list *current,
		t_shell *shell, t_args **env_list, char *input)
{
	args_handle(current->head, shell, env_list, input);
	exit(EXIT_SUCCESS);
}

void	execute_parent(int prev_pipe_fd[2], int next_pipe_fd[2])
{
	close_pipe(prev_pipe_fd);
	prev_pipe_fd[0] = next_pipe_fd[0];
}
*/
