/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_dollardollar.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 18:29:25 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/04 18:22:42 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
int	is_alphanum(char c)
{
	if (c >= 'A' && c <= 'Z')
		return (0);
	if (c >= 'a' && c <= 'z')
		return (0);
	if (c >= '0' && c <= '9')
		return (0);
	if (c == '_')
		return (0);
	if (c == '?')
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

static int	step2split(t_split *s, char *str)
{
	if (s->start < s->i)
	{
		// if (s->result[s->j])
		// 	free(s->result[s->j]);
		s->result[s->j++] = word_dup(str, s->start, s->i);
	}
	s->result[s->j] = NULL;
	return (0);
}

/*
 * Permet d'initialiser la structure, et de traiter la premiere sous chaine
 * jusqu'a la rencontre d'un '$'
*/
static int	init_split(t_split *s, char *str, int step)
{
	if (step == 2)
		return (step2split(s, str));
	s->i = 0;
	s->j = 0;
	s->start = 0;
	s->flag = 0;
	if (step == 1)
	{
		s->result = ft_calloc(1000, sizeof(char *));
		if (!s->result)
			return (-1);
	}
	while (str[s->i] != '$' && str[s->i] != '\0')
		s->i++;
	if (s->i != 0)
	{
		// if (s->result[s->j])
		// 	free(s->result[s->j]);
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
