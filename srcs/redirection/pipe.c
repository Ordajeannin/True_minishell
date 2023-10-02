/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 15:16:42 by ajeannin          #+#    #+#             */
/*   Updated: 2023/10/02 18:59:47 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet d'executer les commandes en pipeline
 * sans connaitre a l'avance le nombre de sous-chaines
 * (sa norme constitue mon plus grand cauchemard)
 * deso j'ai rajoute des lignes hors norme, le cauchemar se prolonge ...
*/
static void	execute_command(t_args_list **stock, t_shell *shell, \
		t_args **env_list, t_args *list)
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
			args_handle(current->head, shell, env_list);
			free_everything(shell, list, *env_list);
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
		if (WIFEXITED(status) != 0 && WEXITSTATUS(status) != 0)
		{
			change_error(env_list, shell, WEXITSTATUS(status));
			shell->error = WEXITSTATUS(status);
		}
	}
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
	execute_command(&stock, shell, env_list, list);
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
		if (flag)
			free(input);
		else
			break ;
	}
	return (input);
}

/*
 * Permet de normer chek_if_there_is_a_lost_pipe
*/
static char	*combine_input_with_new_one(char *input, int lenght)
{
	char	*new_input;
	char	*combined;

	new_input = call_readline("> ");
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

/*
 * Permet de verifier si l'input ne se termine pas par un pipe
 * (sans prendre en compte les espaces)
 * Auquel cas on doit de nouveau demander un input a l'utilisateur,
 * A concatener avec le precedent
*/
char	*check_if_there_is_a_lost_pipe(char *input)
{
	int	lenght;
	int	i;

	lenght = ft_strlen(input);
	i = lenght - 1;
	while (i >= 0 && input[i] == ' ')
		i--;
	if (i >= 0 && input[i] == '|')
		return (combine_input_with_new_one(input, lenght));
	else
		return (input);
}

/*
 * Last Stable Version,
 * a conserver au cas ou la version normee ne soit pas bug-proof
*/
/*
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
*/
