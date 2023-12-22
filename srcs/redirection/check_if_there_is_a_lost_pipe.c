/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_if_there_is_a_lost_pipe.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 19:35:02 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 21:27:31 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_multiple_pipe(char *input)
{
	int	flag;

	flag = 0;
	while (*input)
	{
		if (*input != ' ' && *input != '\t' && *input != '|')
			flag = 0;
		if (flag == 1 && *input == '|')
			return (-1);
		if (*input == '|')
			flag = 1;
		input++;
	}
	return (0);
}

static char	*help_help(int i, int lenght, char *input2)
{
	while (1)
	{
		lenght = ft_strlen(input2);
		i = lenght - 1;
		while (i >= 0 && input2[i] == ' ')
			i--;
		if (i >= 0 && input2[i] == '|')
		{
			if (check_multiple_pipe(input2) == 0)
				input2 = combine_input_with_new_one(input2, lenght);
			else
			{
				add_history(input2);
				printf("syntax error near unexpected token\n");
				exit(2);
			}
		}
		else
			break ;
	}
	return ("pas null");
}

char	*help_sctialp(char *input, int *pipes, char *input2)
{
	int	lenght;
	int	i;

	lenght = 0;
	i = 0;
	input2 = input;
	signal(SIGINT, &signal_heredoc);
	close(pipes[0]);
	close_pipefd(pipes, 0);
	if (help_help(i, lenght, input2) == NULL)
		return (NULL);
	write(pipes[1], input2, ft_strlen(input2));
	close(pipes[1]);
	exit(0);
}

static void	exit_lost_pipe(char *str)
{
	perror(str);
	ft_malloc(0, FREE);
	exit(EXIT_FAILURE);
}

char	*check_if_there_is_a_lost_pipe(char *input)
{
	pid_t	pid;
	int		status;
	int		pipes[2];
	char	*input2;

	input2 = input;
	if (pipe(pipes) == -1)
		exit_lost_pipe("pipe");
	pid = fork();
	if (pid == -1)
		exit_lost_pipe("fork");
	if (pid == 0)
	{
		if (help_sctialp(input, pipes, input2) == NULL)
			return (NULL);
	}
	waitpid(pid, &status, 0);
	if (WEXITSTATUS(status) == 2)
	{
		set_error_nb(WEXITSTATUS(status), YES);
		return (NULL);
	}
	close(pipes[1]);
	input2 = get_result(pipes);
	close(pipes[0]);
	return (input2);
}
