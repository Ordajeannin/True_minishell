/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delim.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 11:42:36 by ajeannin          #+#    #+#             */
/*   Updated: 2023/10/02 17:53:58 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	tempfile(char *str)
{
	int	temp_fd;

	temp_fd = open("tempfile.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (temp_fd == -1)
	{
		perror("open");
		return ;
	}
	if (write(temp_fd, str, ft_strlen(str)) == -1)
	{
		perror("write");
		close(temp_fd);
		return ;
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
		result = (char *)malloc(buffer_lenght);
	else
	{
		temp = (char *)malloc(*input_size + buffer_lenght);
		if (!temp)
		{
			free(result);
			return (NULL);
		}
		ft_strcpy(temp, result);
		free(result);
		result = temp;
	}
	if (!result)
		return (NULL);
	return (malediction(line, result, input_size));
}

/*
 * redirection <<
 * en l'etat, et isole du programme, interagit avec l'utilisateur
 * et recupere son input sous forme de str\nstr\nstr\n
 * jusqu au delim    (*list)->next->str
 * prochaine etape : gerer cet *str pour en faire l'input de la commande
 * (fichier temporaire?)
*/
void	plus_de_nouvelle(const char *str)
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
		free(line);
		if (result == NULL)
			return ;
	}
	if (line)
		free(line);
	if (result)
		return (tempfile(result));
}

int	handle_heredoc(t_args **input)
{
	t_args	*current;
	t_args	*prev;
	t_args	*next;
	t_args	*stock;

	current = *input;
	prev = NULL;
	next = NULL;
	stock = NULL;
	if (input == NULL || *input == NULL)
		return (1);
	while (current != NULL)
	{
		if (current->token == TOKEN_DELIM)
		{
			if (current->next != NULL)
				add_arg(&stock, current->next->str, TOKEN_DELIM);
			else
				add_arg(&stock, NULL, -66);
			next = current->next;
			free(current);
			if (prev == NULL)
				*input = next;
			else
				prev->next = next->next;
			if (next != NULL)
			{
				current = next->next;
				free(next);
			}
			else
			{
				current = NULL;
				if (prev != NULL)
					prev->next = NULL;
			}
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	printf("----------------- RESULTAT HEREDOC --------------------\n");
	if (stock != NULL)
		print_args_list(&stock);
	printf("--------------- INPUT APRES HEREDOC -------------------------\n");
	if (input != NULL)
		print_args_list(input);
	return (0);
}

			





/*
 * fonction initiale, a conserver si comportement indesirable
 *
char	*plus_de_nouvelle(const char *str)
{
	char	*result;
	char	*line;
	int		input_size;
	int		buffer_lenght;

	result = NULL;
	line = NULL;
	input_size = 0;
	buffer_lenght = 0;
	while (1)
	{
		line = readline("> ");
		if (line == NULL || ft_strcmp(line, str) == 0)
			break ;
		buffer_lenght = ft_strlen(line) + 1;
		if (result == NULL)
			result = (char *)malloc(buffer_lenght);
		else
			result = (char *)ft_realloc(result, input_size + buffer_lenght);
		if (result == NULL)
		{
			free(line);
			return (NULL);
		}
		if (input_size == 0)
			ft_strcpy(result, line);
		else
		{
			ft_strcat(result, "\n");
			ft_strcat(result, line);
		}
		input_size += buffer_lenght;
		free(line);
	}
	if (line)
		free(line);
	return (result);
}
*/
