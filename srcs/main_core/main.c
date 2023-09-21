/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 20:36:50 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/18 18:38:54 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet a main d'etre a moins de 25 lines
 * Fonction purement utilitaire, ne pas garder dans le rendu final
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
 * Suite du main #2
*/
static void	main_ter(t_args *list, t_shell *shell, t_args **env_list, \
	char *input)
{
	if (is_correct_format(&list) == 0)
	{
		is_there_a_redirection(&list);
		args_handle(list, shell, env_list, input);
	}
}

/* 
 * Suite du main.
*/
static void	main_bis(char *input, t_args *list, t_args *env_list, \
	t_shell *shell)
{
	int	saved_stdout;
	int	saved_stdin;

	if (g_error != 0)
	{
		change_error(&env_list, g_error);
		g_error = 0;
	}
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	from_input_to_list_of_args(input, &list, &env_list);
	if (list)
		main_ter(list, shell, &env_list, input);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		perror("Failed to restore standard output\n");
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		perror("Failed to restore standard input\n");
	close(saved_stdout);
	free(input);
	clear_args_list(&list);
}

/*
 * Actions de la boucle ATM
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
	char	*user;
	t_shell	shell;

	(void)ac;
	g_error = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	ft_gain_place(av, &list, &input, &env_list);
	if (set_env(&env_list, env, &shell) == -1)
		return (-1);
	user = shell.user;
	input = readline(prompt_cmd(&shell, user));
	if (input == NULL)
		ft_exit(input, list, env_list, &shell);
	add_history(input);
	main_bis(input, list, env_list, &shell);
	shell.input_bis = input;
	is_minishell(shell, env_list, list, user);
	return (0);
}

/* 
 * Boucle principale minishell
 * Affiche le prompt
 * Ajoute la cmd a l'historique si besoin
 * Exit si CTRL-D
*/
int	is_minishell(t_shell shell, t_args *env_list, t_args *list, char *user)
{
	char	*input;

	input = NULL;
	while (1)
	{
		input = readline(prompt_cmd(&shell, user));
		if (input == NULL)
			ft_exit(input, list, env_list, &shell);
		if (!(ft_strcmp(shell.input_bis, input) == 0 \
			&& ft_strlen(shell.input_bis) == ft_strlen(input)))
			add_history(input);
		shell.input_bis = ft_strdup(input);
		main_bis(input, list, env_list, &shell);
	}
}
