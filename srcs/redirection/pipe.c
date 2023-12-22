/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 15:16:42 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 17:03:41 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet d'executer les commandes en pipeline
 * sans connaitre a l'avance le nombre de sous-chaines
 * (sa norme constitue mon plus grand cauchemard)
 *
 * ft_malloc(0, FREE) si pid == 0? pourquoi?
*/
static void	execute_command(t_args_list **stock, t_shell *shell, \
		t_args **env_list)
{
	t_args_list	*current;
	int			prev_pipe[2];
	int			pipe_fds[2];
	pid_t		pid;
	int			status;

	current = *stock;
	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	status = 0x0;
	while (current != NULL)
	{
		if (ft_atoi(find_a("SHLVL", *env_list)->str + 6) > 1)//sert a quoi ?
		{	printf("SERT A QQ CHOSE !\n");
			g_error = 2;
			signal(SIGQUIT, signal_handler);
			//Probleme signaux: SIGQUIT est ignore dans execve avant qu'il soit ignore ici
		}
		if (pipe(pipe_fds) == -1)
		{
			perror("pipe");
			ft_readline(NULL, NO);
			ft_malloc(0, FREE);
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			ft_readline(NULL, NO);
			ft_malloc(0, FREE);
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
			args_handle(current->head, shell, env_list);
			ft_malloc(0, FREE);
			exit(shell->error);
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
	while (wait(&status) > 0)
	{
		signal(SIGQUIT, SIG_IGN);
		if (WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0)
		{
			set_error_nb(WEXITSTATUS(status), YES);
			// change_error(env_list, shell, WEXITSTATUS(status));
			// shell->error = WEXITSTATUS(status);
		}
	}
	if (g_error == 2)
		g_error = 0;
}

/*
 * Si l'input contient n pipe, creera n + 1 sublists
 * et les stockera dans une nouvelle liste chainee, **stock
 * en vu d'etre execute simultanement par execute_command
*/
void	create_sublists(t_args *list, t_shell *shell, t_args **env_list)
{
	t_args_list	*stock;

	stock = stock_sublist(&list);
	print_sublists(stock);
	if (stock->next != NULL)
		execute_command(&stock, shell, env_list);
	else
		args_handle(list, shell, env_list);
}

/*
 * Permet de normer combine_input_with_new_one
 * (oui, c est un comble)
 * Dans les faits, va s'assurer que le nouvel input ne soi pas vide
 * ou constitue d'espaces uniquement
 * (si c est le cas, repropose un prompt et ne cat pas)
*/
static char	*call_readline(char *prompt)
{
	char	*input;
	char	*ptr;
	int		flag;

	flag = 1;
	while (1)
	{
		input = readline(prompt);
		if (!input)
			return (NULL);
		ptr = input;
		while (*ptr)
		{
			if (*ptr++ != ' ')
				flag = 0;
		}
		if (!flag)
			break ;
	}
	return (input);
}

/*
 * Permet de normer check_if_there_is_a_lost_pipe
*/
static char	*combine_input_with_new_one(char *input, int lenght)
{
	char	*new_input;
	char	*combined;

	new_input = call_readline("> ");
	if (!new_input)
		return (NULL);
	combined = (char *)ft_malloc(lenght + ft_strlen(new_input) + 2, ALLOC);
	if (!combined)
	{
		perror("malloc");
	 	return (NULL);
	 }
	ft_strcpy(combined, input);
	ft_strcat(combined, " ");
	ft_strcat(combined, new_input);
	return (combined);
}

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
/*
 * Permet de verifier si l'input ne se termine pas par un pipe
 * (sans prendre en compte les espaces)
 * Auquel cas on doit de nouveau demander un input a l'utilisateur,
 * A concatener avec le precedent
 * Boucle tant que les inputs se terminent par un pipe
*/
char	*check_if_there_is_a_lost_pipe(char *input)
{
	int		lenght;
	int		i;
	char	*input2;
	pid_t	pid;
	int		status;
	int		pipes[2];

	input2 = input;
	pipe(pipes);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, &signal_heredoc);
		close (pipes[0]);
		close_pipefd(pipes, 0);
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
					return (NULL);
				}
			}
			else
				break ;
		}
		write(pipes[1], input2, ft_strlen(input2));
		close(pipes[1]);
		// ft_malloc(0, FREE);
		exit(1);
	}
	waitpid(pid, &status, 0);
	close(pipes[1]);
	input2 = get_result(pipes);
	printf("NANI ?:%s\n", input2);
	close(pipes[0]);
	return (input2);
}
