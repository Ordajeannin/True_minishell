/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_dollardollar.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:29:25 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/14 20:42:06 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de... gagner de la place
*/
typedef struct s_split
{
	int		i;
	int		j;
	int		start;
	int		flag;
	char	**result;
}	t_split;

/*
 * alloue une sous chaine, et l'extrait de str
*/
static char	*word_dup(char *str, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc(sizeof(char) * (finish - start + 1));
	if (!word)
		return (NULL);
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

/*
 * gere les cas particuliers
 * $USERstrSTR = $USERstrSTR
 * de meme que
 * $USER_123 = $USER_123
 * 
 * mais :warning:
 * $USER&^%
 * -> $USER
 * -> &
 * -> ^
 * -> %
 *  ect...
*/
static int	is_alphanum(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (0);
	if (c >= 'a' && c <= 'z')
		return (0);
	if (c >= '0' && c <= '9')
		return (0);
	if (c == '_')
		return (0);
	else
		return (-1);
}

/*
 * Permet de reduire la taille de la fonction principale
 * mets a jour certaines variables
 * respecte l'ordre des condition, cf 'step'
*/
static void	update_split(t_split *s, int step)
{
	if (step == 1)
	{
		s->start = s->i;
		s->flag = 0;
	}
	if (step == 2)
	{
		s->i++;
		s->start = s->i + 1;
	}
	if (step == 3)
	{
		s->flag = 1;
		s->start = s->i;
	}
	if (step == 4)
	{
		s->flag = 1;
		s->start = s->i;
	}
}

/*
 * Permet d'initialiser la structure, et de traiter la premiere sous chaine
 * jusqu'a la rencontre d'un '$'
*/
static int	init_split(t_split *s, char *str, int step)
{
	if (step == 2)
	{
		if (s->start < s->i)
			s->result[s->j++] = word_dup(str, s->start, s->i);
		s->result[s->j] = NULL;
		return (0);
	}
	s->i = 0;
	s->j = 0;
	s->start = 0;
	s->flag = 0;
	s->result = malloc(sizeof(char *) * 1000);
	if (!s->result)
		return (-1);
	while (str[s->i] != '$' && str[s->i] != '\0')
		s->i++;
	if (s->i != 0)
	{
		s->result[s->j++] = word_dup(str, s->start, s->i);
		s->start = s->i;
	}
	return (0);
}

/*
 * Main fonction
 * cf version + clair ci-dessous
 * retourne un tableau de tableau contenant chaque sous-chaine
 * devant etre traitee par is_env_var
*/
char	**ft_split_arg(char *str)
{
	t_split	s;

	if (init_split(&s, str, 1) == -1)
		return (NULL);
	while (str[s.i] != '\0')
	{
		if (s.flag == 1 && (str[s.i] == '$' || str[s.i] == '\0'
				|| is_alphanum(str[s.i]) == -1))
		{
			s.result[s.j++] = word_dup(str, s.start, s.i);
			update_split(&s, 1);
		}
		if (str[s.i] == '$' && str[s.i + 1] == '$')
		{
			s.result[s.j++] = word_dup(str, s.i, s.i + 2);
			update_split(&s, 2);
		}
		else if (str[s.i] == '$' && s.flag == 0)
			update_split(&s, 3);
		else if (str[s.i] != '$' && s.flag == 0)
			update_split(&s, 4);
		s.i++;
	}
	init_split(&s, str, 2);
	return (s.result);
}

/*
 * version plus clair mais incorrect pour la norme
 * a conserver pour awena
char	**ft_split_arg(char *str)
{
	int		i;
	int		j;
	int		start;
	int		flag;
	char	**result;

	i = 0;
	j = 0;
	start = 0;
	flag = 0;
	result = malloc(sizeof(char *) * 1000);
	if (!result)
		return (NULL);
	while (str[i] != '$' && str[i] != '\0')
		i++;
	if (i != 0)
	{
		result[j++] = word_dup(str, start, i);
		start = i;
	}
	while (str[i] != '\0')
	{
		if (flag == 1 && (str[i] == '$' || str[i] == '\0'
				|| is_alphanum(str[i]) == -1))
		{
			result[j++] = word_dup(str, start, i);
			start = i;
			flag = 0;
		}
		if (str[i] == '$' && str[i + 1] == '$')
		{
			result[j++] = word_dup(str, i, i + 2);
			i++;
			start = i + 1;
		}
		else if (str[i] == '$' && flag == 0)
		{
			flag = 1;
			start = i;
		}
		else if (str[i] != '$' && flag == 0)
		{
			flag = 1;
			start = i;
		}
		i++;
	}
	if (start < i)
		result[j++] = word_dup(str, start, i);
	result[j] = NULL;
	return (result);
}
*/
