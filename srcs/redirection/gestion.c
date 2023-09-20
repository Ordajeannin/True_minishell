/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gestion.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <ajeannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 19:59:31 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/20 19:59:38 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * redirection <
 * Ouvre un fichier (deja existant) du nom de la string de l'arg suivant
 * En "Read Only" !
 * puis redirige l'input vers ce fichier
*/
static void	j_ai_peter_les_plombs(t_args **list)
{
	t_args	*current;
	int		fd;

	current = *list->next;
	fd = open(current->str, O_RDONLY);
	if (fd == -1)
	{
		perror("Failed to open input file");
		return ;
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("Failed to redirect input");
		close(fd);
		return ;
	}
	close(fd);
}

/*
 * redirection >
 * Ouvre un fichier du nom de la string de l'argument suivant
 * Efface son contenu (O_TRUNC)
 * Puis redirige la sortie vers ce fichier
*/ 
static void	sans_abandonner(t_args **list)
{
	t_args	*current;
	int		fd;

	current = (*list)->next;
	fd = open(current->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("Failed to open output file");
		return ;
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Failed to redirect output");
		close(fd);
		return ;
	}
	close(fd);
}

/*
 * redirection >>
 * Ouvre un fichier avec la string de l'argument suivant
 * En mode APPEND (difference avec '>')
 * Puis redirige la sortie vers ce fichier
*/
static void	ni_baisser_les_bras(t_args **list)
{
	t_args	*current;
	int		fd;

	current = (*list)->next;
	fd = open(current->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("Failed to open output file");
		return ;
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Failed to redirect output");
		close(fd);
		return ;
	}
	close(fd);
}

/*
 * Redirection (lol) en fonction du token vers la fonction correspondante
 * < redirection d'entree, fournit l'input de la commande via un fichier
 * > redirection de sortie, redirige la sortie d'une commande dans un fichier
 * (ATTENTION, ecrase le contenu existant)
 * >> redirection de sortie, mais AJOUTE la sortie d'une commande a la fin
 * d'un fichier
 *
 * Toutes les redirections creent les fichiers necessaires. Meme si elles sont
 * remplacees apres, le fichier reste
*/
void	c_est_ma_direction(int token, t_args **list)
{
	if (token == TOKEN_INFILE)
		j_ai_peter_les_plombs(list);
	if (token == TOKEN_OUTFILE)
		sans_abandonner(list);
	if (token == TOKEN_APPEND)
		ni_baisser_les_bras(list);
}
