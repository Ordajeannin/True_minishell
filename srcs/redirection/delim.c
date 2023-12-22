/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 11:42:36 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 17:16:19 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de creer le fichier temporaire,
 * pour stocker l'input de l'utilisateur.
 * (uniquement celui entre les deux derniers delims si multiple)
 * le fichier sera l'entree par defaut des commandes
 * (fichier supprime a la fin de l'execution)
*/
void	tempfile(char *str)
{
	int	temp_fd;

	temp_fd = open("tempfile.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (temp_fd == -1)
	{
		perror("open");
		return ;
	}
	if (str != NULL)
	{
		if (write(temp_fd, str, ft_strlen(str)) == -1)
		{
			perror("write");
			close(temp_fd);
			return ;
		}
	}
	close(temp_fd);
	if (dup2(open("tempfile.txt", O_RDONLY), STDIN_FILENO) == -1)
	{
		perror("dup2");
		return ;
	}
}

/*
 * Suite et Fin redirection <<
 * Permet de concatener l'input precedement recupere avec les precedents
 * s'assure de les separer avec un \n et de update la taille de l'input
 * pour les futurs realloc
*/
static char	*malediction(const char *line, char *result, int *input_size)
{
	if (*input_size == 0)
	{
		ft_strcpy(result, line);
		ft_strcat(result, "\n");
	}
	else
	{
		ft_strcat(result, line);
		ft_strcat(result, "\n");
	}
	*input_size += ft_strlen(line) + 1;
	return (result);
}

/*
 * Suite redirection <<
 * permet de recuperer l'input de l'utilisateur,
 * avant de concat avec les inputs precedents
*/
static char	*batterie_faible(const char *line, char *result, int *input_size)
{
	int		buffer_lenght;
	char	*temp;

	buffer_lenght = ft_strlen(line) + 1;
	temp = NULL;
	if (result == NULL)
		result = (char *)ft_malloc(buffer_lenght, ALLOC);
	else
	{
		temp = (char *)ft_malloc(*input_size + buffer_lenght, ALLOC);
		if (!temp)
			return (NULL);
		ft_strcpy(temp, result);
		result = temp;
	}
	if (!result)
		return (NULL);
	return (malediction(line, result, input_size));
}

/*
 * redirection <<
 * Interagit avec l'utilisateur
 * et recupere son input sous forme de str\nstr\nstr\n
 * jusqu au dernier delim
 * -> stockage dans un fichier temporaire pour en faire l entree par defaut
*/
void	plus_de_nouvelle(const char *str, int *pipes)
{
	char	*result;
	char	*line;
	int		input_size;

	result = NULL;
	line = NULL;
	input_size = 0;
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, str) == 0)
			break ;
		result = batterie_faible(line, result, &input_size);
		if (result == NULL)
			return ;
	}
	write(pipes[1], result, ft_strlen(result));
}

/*
 * Permet de gerer les multiples heredoc
 * Laisse la main a l utilisateur tant que les eof sont definis
 * Si un EOF n'est pas defini,
 * erreur quand c est a son tour d etre pris en compte
 * Si on arrive au dernier EOF, alors stockage de l'input + concatenation,
 * into fichier temporaire qui sera la nouvelle entree par defaut
*/
int	handle_mult_heredoc(t_args **stock, int *pipes)
{
	t_args	*current;
	char	*line;

	if (*stock == NULL)
		return (0);
	current = *stock;
	while (current->next != NULL)
	{
		if (current->str == NULL)
			return (1);
		if (set_error_nb(0, NO) == 130)
			return (0);
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, current->str) == 0)
			current = current->next;
	}
	if (current->str == NULL)
		return (1);
	plus_de_nouvelle(current->str, pipes);
	close(pipes[1]);
	return (0);
}

/*
 *recupere le char* de l'enfant
 *
*/
char	*get_result(int *pipes)
{
	char	*result;
	char	*tmp;

	tmp = ft_strdup("");
	result = ft_strdup("");
	while (tmp)
	{
		tmp = get_next_line(pipes[0]);
		if (tmp)
			result = ft_strjoin(result, tmp);
	}
	return (result);
}

/*
 * Permet de gerer l'encapsulation des heredoc
 * Creation d'une liste chainee qui stockera les EOF des << rencontres
 * si << :
 * supression des maillons current et next (si il existe) de la chaine principal
 * creation d'un maillon pour stock
 * (valeur != en fonction de la presence ou non d'un EOF)
 * mise a jour du prev->next pour assurer la stabilite
*/
int	handle_heredoc(t_args **input)
{
	t_args	*current;
	t_args	*prev;
	t_args	*next;
	t_args	*stock;
	pid_t	pid;
	int		status;
	int		pipes[2];

	current = *input;
	prev = NULL;
	next = NULL;
	stock = NULL;
	if (input == NULL || *input == NULL)
		return (0);
	while (current != NULL)
	{
		if (current->token == TOKEN_DELIM)
		{
			if (current->next != NULL)
			{
				add_arg(&stock, current->next->str, TOKEN_DELIM);
				next = current->next;
				if (prev == NULL)
					*input = next;
				else
					prev->next = next->next;
				if (next != NULL)
					current = next->next;
				else
				{
					current = NULL;
					if (prev != NULL)
						prev->next = NULL;
				}
			}
			else
			{
				add_arg(&stock, NULL, -66);
				if (prev != NULL)
					prev->next = NULL;
				else
					*input = NULL;
				break ;
			}
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	pipe(pipes);
	pid = fork();
	if (pid == 0)
	{
		close(pipes[0]);
		close_pipefd(pipes, 0);
		signal(SIGINT, &signal_heredoc);
		if (handle_mult_heredoc(&stock, pipes) == 1)
		{
			perror("syntax error near unexpected token");
			exit (2);
		}
		exit(0);
	}
	else
	{
		close(pipes[1]);
		waitpid(pid, &status, 0);
		if (WEXITSTATUS(status) == 2 || WEXITSTATUS(status) == 130)
		{
			close(pipes[0]);
			set_error_nb(WEXITSTATUS(status), YES);
			return (2);
		}
		if (stock != NULL)
			tempfile(get_result(pipes));
		close(pipes[0]);
	}
	return (0);
}
