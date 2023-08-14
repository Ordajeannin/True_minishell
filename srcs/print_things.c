/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_things.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 16:28:28 by asalic            #+#    #+#             */
/*   Updated: 2023/08/14 19:22:44 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet d'afficher les details extraits pour chaque arguments, issue de input
 * Parcourt la liste chainee, et affiche :
 * 1) Le "rang" de l'argument
 * 2) Son contenu
 * 3) Comment nous l'avons interprete
 * 
 * Fonction purement utilitaire, ne pas garder dans le rendu final
*/
void	print_args_list(t_args **list)
{
	t_args	*current;
	int		i;

	current = *list;
	i = 0;
	while (current != NULL)
	{
		if (current->str != NULL)
			ft_printf("\nargument %d\nstring: %s\ntoken: %d\n\n",
				i, current->str, current->token);
		else
			ft_printf("\nargument %d\nstring: NULL\ntoken: %d\n\n",
				i, current->token);
		i++;
		current = current->next;
	}
}

/* 
 * Fonction Toto
 * Permets de styliser une chaine de caractere
*/
static void	bf_prd(char *str, int d, char *color)
{
	char	*str_cara;
	char	*color_strdup;
	int		idx_str;
	int		idx;

	color_strdup = ft_strdup(color);
	idx_str = 0;
	write(1, color_strdup, ft_strlen(color));
	free(color_strdup);
	while (str[idx_str])
	{
		idx = 0;
		while (idx < d)
			idx++ ;
		str_cara = (char *)malloc(2 * sizeof(char));
		str_cara[0] = str[idx_str];
		str_cara[1] = '\0';
		write(1, str_cara, 1);
		free(str_cara);
		idx_str++ ;
	}
	color_strdup = ft_strdup("\033[0m");
	write(1, color_strdup, ft_strlen(color_strdup));
	free(color_strdup);
}

/* 
 * Style et couleur du prompt.
 * Ajout de couleurs a l'invite de commande.
*/
// void	shell_style(t_shell *shell)
// {
// 	if (shell->user != NULL)
// 	{
// 		bf_prd(shell->user, 0, "\033[1;32m");
// 		ft_printf(":");
// 	}
// }

char	*prompt_cmd(t_shell *shell, char *user)
{
	char	*prompt;

	prompt = NULL;
	if (user != NULL)
		prompt = ft_strjoin(user, ":");
	if (shell->is_pwd != NULL)
		prompt = ft_strjoin(prompt, shell->is_pwd);
	prompt = ft_strjoin(prompt, ">");
	return (prompt);
}
