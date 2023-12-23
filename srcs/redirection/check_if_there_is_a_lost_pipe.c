/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_if_there_is_a_lost_pipe.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 19:35:02 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/23 16:13:10 by ajeannin         ###   ########.fr       */
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
	return (input2);
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
	input2 = help_help(i, lenght, input2);
	write(pipes[1], input2, ft_strlen(input2));
	close(pipes[1]);
	exit(0);
}

static int	exit_lost_pipe(char *str, char *flag, int status)
{
	if (ft_strcmp(flag, "EXIT") == 0)
	{
		perror(str);
		ft_malloc(0, FREE);
		exit(EXIT_FAILURE);
	}
	else
	{
		if (WEXITSTATUS(status) == 2)
		{
			set_error_nb(WEXITSTATUS(status), YES);
			return (1);
		}
		return (0);
	}
}

char	*check_if_there_is_a_lost_pipe(char *input)
{
	pid_t	pid;
	int		status;
	int		pipes[2];
	char	*input2;

	input2 = input;
	if (pipe(pipes) == -1)
		exit_lost_pipe("pipe", "EXIT", status);
	pid = fork();
	if (pid == -1)
		exit_lost_pipe("fork", "EXIT", status);
	if (pid == 0)
	{
		if (help_sctialp(input, pipes, input2) == NULL)
			return (NULL);
	}
	waitpid(pid, &status, 0);
	if (exit_lost_pipe("str", "ERROR", status) == 1)
		return (NULL);
	close(pipes[1]);
	input2 = get_result(pipes);
	close(pipes[0]);
	return (input2);
}
