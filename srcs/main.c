/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <ajeannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 20:36:50 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/05 17:03:45 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 *Gestionnaire de commandes.
 *Recupere l'arguments et la commande separement.
 *Traite en fonction de la commande enregistree
*/
void	args_handle(t_args **list)
{
	t_args	*current_list;

	current_list = *list;
	if (ft_strncmp(current_list->str, "echo", ft_strlen(current_list->str))
		== 0)
		ft_echo(current_list);
	else if (ft_strncmp(current_list->str, "cd", ft_strlen(current_list->str))
		== 0)
		ft_cd(current_list);
	else if (ft_strncmp(current_list->str, "pwd", ft_strlen(current_list->str))
		== 0)
		ft_pwd();
	else if (ft_strncmp(current_list->str, "env", ft_strlen(current_list->str))
		== 0)
		ft_env();
}

/*
 * Permet d'afficher les details extraits pour chaque arguments, issue de input
 * Parcourt la liste chainee, et affiche :
 * 1) Le "rang" de l'argument
 * 2) Son contenu
 * 3) Comment nous l'avons interprete
 * 
 * Fonction purement utilitaire, ne pas garder dans le rendu final
*/
static void	print_args_list(t_args **list)
{
	t_args	*current;
	int		i;

	current = *list;
	i = 0;
	while (current != NULL)
	{
		ft_printf("\nargument %d\nstring: %s\ntoken: %d\n\n",
			i, current->str, current->token);
		i++;
		current = current->next;
	}
}

/*
 * Actions de la boucle ATM
 *
 * 1) readline ("prompt")
 * 2) splitage de l'input en tableau de chaine
 * 3) analyse de chaque chaine, attribution des tokens
 * 4) affichage de l'analyse (temporaire)
 * 5) ?
 * 6) gere l'historique de commande
 * 7) free le precedent input avant de reproposer le prompt
 * 8) vide la liste d'arguments, mais conserve le pointeur
*/
int	main(int ac, char **av, char **env)
{
	char	*input;
	t_args	*list;
	t_shell	shell;

	(void)ac;
	(void)av;
	list = NULL;
	input = NULL;
	if (handle_env(env, &shell) == -1)
		return (-1);
	while (1)
	{
		input = readline("minishell>");
		shell.input = ft_split(input, ' ');
		from_input_to_list_of_args(shell.input, &list);
		print_args_list(&list);
		args_handle(&list);
		add_history(input);
		parsing_input(&input);
		free(input);
		clear_args_list(&list);
	}
	return (0);
}
