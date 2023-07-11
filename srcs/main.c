/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 20:36:50 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/11 12:23:47 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 *Gestionnaire de commandes.
 *Recupere l'arguments et la commande separement.
 *Traite en fonction de la commande enregistree
*/
static void	args_handle(t_args *list, t_shell *shell, t_args **env_list)
{
	t_args	*current_env;

	current_env = *env_list;
	if (list == NULL)
		return ;
	else if (ft_strncmp(list->str, "echo", ft_strlen(list->str))
		== 0)
		ft_echo(list);
	else if (ft_strncmp(list->str, "cd", ft_strlen(list->str))
		== 0)
		ft_cd(list, shell, current_env);
	else if (ft_strncmp(list->str, "pwd", ft_strlen(list->str))
		== 0)
		ft_pwd();
	else if (ft_strncmp(list->str, "env", ft_strlen(list->str))
		== 0)
		ft_env(list, env_list);
	else if (ft_strncmp(list->str, "unset",
			ft_strlen(list->str)) == 0)
		ft_unset(list, shell, current_env);
	else if (ft_strncmp(list->str, "export",
			ft_strlen(list->str)) == 0)
		ft_export(list, shell, env_list);
	else
		all_cmd(list, shell, &list);
}

/* 
 * Initialise liste d'env 
*/
static int	set_env(t_args **env_list, char **env, t_shell *shell)
{
	int			i;
	t_args		*current;

	if (handle_env(env, shell) == -1)
		return (-1);
	i = 0;
	while (env[i])
	{
		add_arg(env_list, env[i], 0);
		i++;
	}
	i = 0;
	current = *env_list;
	while (env[i])
	{
		current->str = env[i++];
		current = current->next;
	}
	return (0);
}

/*
 * Permet a main d'etre a moins de 25 lines
 * Fonction purement tilitaire, ne pas garder dans le rendu final
*/
static void	ft_gain_place(char **av, t_args **list, char **input,
		t_args **env_list)
{
	*list = NULL;
	*input = NULL;
	*env_list = NULL;
	(void)av;
}

/*
 * Actions de la boucle ATM
 *
 * 1) readline ("prompt")
 * 2) splitage de l'input en tableau de chaine
 * 3) analyse de chaque chaine, attribution des tokens
 * 4) affichage de l'analyse (temporaire)
 * 5) gestionnaire de commandes
 * 6) gere l'historique de commande
 * 7) free le precedent input avant de reproposer le prompt
 * 8) vide la liste d'arguments, mais conserve le pointeur
*/
int	main(int ac, char **av, char **env)
{
	char	*input;
	t_args	*list;
	t_args	*env_list;
	t_shell	shell;

	(void)ac;
	ft_gain_place(av, &list, &input, &env_list);
	if (set_env(&env_list, env, &shell) == -1)
		return (-1);
	while (1)
	{
		shell_style(&shell);
		input = readline("minishell>");
		add_history(input);
		from_input_to_list_of_args(input, &list);
		if (list)
		{
			print_args_list(&list);
			args_handle(list, &shell, &env_list);
		}
		free(input);
		clear_args_list(&list);
	}
	return (0);
}
