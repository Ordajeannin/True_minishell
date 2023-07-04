/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 20:36:50 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/04 14:55:46 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 *Gestionnaire de commandes.
 *Recupere l'arguments et la commande separement.
 *(A revoir : enlever les details lies a 'echo' et les ajouter 
 *aux fonctions faites pour echo)
 *(i.e, faire de ce gestionnaire de commandes un reel gestionnaire de 
 *commandes globales, pas au cas par cas)
*/
void	args_handle(char *input)
{
	char	**tab_args;

	tab_args = malloc(3 * sizeof(char));
	tab_args[0] = until_char(input, ' ');
	tab_args[1] = ft_strchr(input, '"');
	if (tab_args[1] == NULL)
		tab_args[1] = ft_strchr(input, 39);
	if (ft_strncmp(tab_args[0], "echo", ft_strlen(tab_args[0])) == 0)
		ft_echo(tab_args[1]);
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_env	envcpy;

	(void)ac;
	(void)av;
	input = NULL;
	if (handle_env(env, &envcpy) == -1)
		return (-1);
	while (1)
	{
		input = readline("minishell>");
		args_handle(input);
		add_history(input);
		free(input);
	}
	return (0);
}
