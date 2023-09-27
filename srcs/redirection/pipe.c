/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 15:16:42 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/27 17:23:13 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	execute_command(t_args_list **stock, t_shell *shell, \
		t_args **env_list, char *input)
{
	t_args_list	*current = *stock;
	int			prev_pipe[2] = {-1, -1};
	int			pipe_fds[2];
	pid_t		pid;

	while (current != NULL)
	{
		if (pipe(pipe_fds) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
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
			args_handle(current->head, shell, env_list, input);
			exit(EXIT_SUCCESS);
		}
		else
		{
			close(pipe_fds[1]);
			if (prev_pipe[0] != -1)
				close(prev_pipe[0]);
			prev_pipe[0] = pipe_fds[0];
		}
		current = current->next;
	}
	while (wait(NULL) > 0)
		;
}

void	create_sublists(t_args *list, t_shell *shell, t_args **env_list, \
		char *input)
{
	t_args_list	*stock;

	stock = stock_sublist(&list);
	print_sublists(stock);
	execute_command(&stock, shell, env_list, input);
}

char	*check_if_there_is_a_lost_pipe(char *input)
{
	char	*new_input;
	char	*combined;
	int		lenght;
	int		i;

	lenght = ft_strlen(input);
	i = lenght - 1;
	while (i >= 0 && input[i] == ' ')
		i--;
	if (i >= 0 && input[i] == '|')
	{
		new_input = readline("> ");
		if (!new_input)
			return (NULL);
		combined = (char *)malloc(lenght + ft_strlen(new_input) + 2);
		if (!combined)
		{
			perror("malloc");
			free(new_input);
			return (NULL);
		}
		ft_strcpy(combined, input);
		ft_strcat(combined, " ");
		ft_strcat(combined, new_input);
		free(new_input);
		return (combined);
	}
	else
		return (input);
}
