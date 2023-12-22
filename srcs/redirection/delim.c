/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 11:42:36 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 17:42:09 by ajeannin         ###   ########.fr       */
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
int	handle_multi_heredoc(t_args **stock, int *pipes)
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