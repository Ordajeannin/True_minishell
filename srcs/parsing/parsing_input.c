/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/06 20:14:29 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * :WARNING:
 * Cet ensemble de fonctions ne gere pas encore les "str", 'str', et les "<<>>"
 * Et n'est pas implemente au programme.
 * Work In Progress, test en local, ne pas effacer le main
*/
static void	delimit_to_token(char *str, int i)
{
	if (strcmp(str, " ") != 0)
		printf("%d delimitateur: %s\n", i, str);
	else
		return ;
}

/*
 * Permet de calculer la partie de input composee uniquement de delimitateurs
*/
static size_t	ft_strspn(const char *input, char **accept)
{
	char		**a;
	size_t		count;
	int			accepted;

	count = 0;
	while (*input)
	{
		a = accept;
		accepted = 0;
		while (*a != NULL)
		{
			if (strncmp(input, *a, strlen(*a)) == 0)
			{
				delimit_to_token(*a, 1);
				accepted = 1;
				break ;
			}
			a++;
		}
		if (!accepted)
			return (count);
		count++;
		input++;
	}
	return (count);
}

/*
 * A l'inverse, renvoie la longueur de la partie de input qui ne contient
 * aucun delimitateur
*/
static size_t	ft_strcspn(const char *input, char **reject)
{
	char		**r;
	size_t		count;
	int			rejected;

	count = 0;
	while (*input)
	{
		r = reject;
		rejected = 0;
		while (*r != NULL)
		{
			if (strncmp(input, *r, strlen(*r)) == 0)
			{
				rejected = 1;
				break ;
			}
			r++;
		}
		if (rejected)
			return (count);
		count++;
		input++;
	}
	return (count);
}

/*
 * reduit la taille de ft_strtok
*/
static void	help_strtok(char *token_end, char **last_token)
{
	*token_end = '\0';
	*last_token = token_end + 1;
}

/*
 * Permet de decouper input en une serie de tokens
 * en utilisant les delimitateurs specifies
 * Chaque appel de la fonction retourne un token de la chaine jusqu a
 * ce que tous soient extraits
 * Si tous les tokens ont ete extraits, retourne NULL
 * Sinon, renvoie un pointeur vers le debut du token extrait
*/
char	*ft_strtok(char *input, char **delim)
{
	static char	*last_token = NULL;
	char		*token_end;

	if (input != NULL)
		last_token = input;
	else
	{
		if (last_token == NULL)
			return (NULL);
		input = last_token;
	}
	input += ft_strspn(input, delim);
	if (*input == '\0')
	{
		last_token = NULL;
		return (NULL);
	}
	token_end = input + ft_strcspn(input, delim);
	if (*token_end != '\0')
		help_strtok(token_end, &last_token);
	else
		last_token = NULL;
	return (input);
}

/*
int main()
{
    char *delim[] = {" ", "\t", "<<", "<", ">>", ">", NULL};
    char input[] = "echo     <hello |  <world>output";

	printf("input : %s\n", input);
    char *token = ft_strtok(input, delim);
    while (token != NULL)
    {
        printf("Token: %s\n", token);
        token = ft_strtok(NULL, delim);
    }

    return 0;
}
*/
