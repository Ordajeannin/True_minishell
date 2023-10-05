/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 20:36:50 by ajeannin          #+#    #+#             */
/*   Updated: 2023/10/05 15:00:43 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static void	print_member(const char *name, const void *member, int type)
// {
// 	if (type == 's')
// 		printf("This is the %s : %s\n", name, (char *)member);
// 	else if (type == 'i')
// 		printf("This is the %s : %i \n", name, *(int *)member);
// 	else if (type == 'a')
// 		printf("This is the %s : %p \n", name, member);
// }

// void print_shell(t_shell *shell)
// {
// 	print_member("home", shell->home, 's');
// 	print_member("path", shell->path, 's');
// 	print_member("cmd_paths", shell->cmd_paths, 'a');
// }

/*
 * Suite du main #2
*/
static void	main_ter(t_args *list, t_shell *shell, t_args **env_list)
{
	if (is_correct_format(&list) == 0)
	{
		is_there_a_redirection(&list);
		//args_handle(list, shell, env_list);
		create_sublists(list, shell, env_list);
	}
}

/* 
 * Suite du main.
*/
static int	main_bis(char *input, t_args *list, t_args *env_list, \
	t_shell *shell)
{
	int		saved_stdout;
	int		saved_stdin;

	if (g_error != 0)
	{
		if (!change_error(&env_list, shell, g_error))
		{
			free(input);
			return (1);
		}
		g_error = 0;
	}
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (from_input_to_list_of_args(input, &list, &env_list) == 1)
	{
		free(input);
		return (1);
	}
	free(input);
	if (list)
		main_ter(list, shell, &env_list);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		perror("Failed to restore standard output\n");
	clear_args_list(&list);
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		perror("Failed to restore standard input\n");
	if (access("tempfile.txt", F_OK != -1))
		unlink("tempfile.txt");
	close(saved_stdout);
	return (0);
}

/* 
 * Init shell->pwd au debut du main
 * Ajout de la premiere commande a l'historique
*/
/*static void	little_more_main(t_shell shell, char **input)
{
	char	buf[1024];

	shell.is_pwd = getcwd(buf, sizeof(buf));
	shell.pwd = shell.is_pwd;
	*input = check_if_there_is_a_lost_pipe(*input);
	if (input != NULL)
		add_history(*input);
}
*/
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
/*
int	main(int ac, char **av, char **env)
{
	char	*input;
	t_args	*list;
	t_args	*env_list;
	t_shell	shell;
	char	*username;
	char	*prompt_char;

	(void)ac;
	ft_bzero(&shell, sizeof shell);
	ft_gain_place(av, &list, &input, &env_list);
	if (set_env(&env_list, env, &shell) == -1)
		return (-1);
	username = get_username(&env_list, &shell);
	prompt_char = prompt_cmd(&shell, username);
	input = readline(prompt_char);
	if (input == NULL)
	{
		free(username);
		free(prompt_char);
		free(input);
		ft_exit(list, env_list, &shell);
	}
	little_more_main(shell, &input);
	ft_printf("je vaux : %s\n", input);
	if (input != NULL)
	{
		shell.input_bis = ft_strdup(input);
		if (main_bis(input, list, env_list, &shell) == 1)
		{
			free(username);
			free(prompt_char);
			free(input);
			free_everything(&shell, list, env_list);
			return (1);
		}
	}
	ft_printf("je suis la \n");
	free(input);
	free(prompt_char);
	is_minishell(&shell, env_list, list, username);
	free(username);
	return (0);
}*/

/*
 *(!) Main ne gere plus l'input
*/
int	main(int ac, char **av, char **env)
{
	t_args	*list;
	t_args	*env_list;
	t_shell	shell;
	char	*username;

	(void)ac;
	ft_bzero(&shell, sizeof shell);
	ft_gain_place(av, &list, &env_list);
	if (set_env(&env_list, env, &shell) == -1)
		return (-1);
	username = get_username(&env_list, &shell);
	if (is_minishell(&shell, env_list, list, username) == 2)
	{
		free(username);
		ft_exit(list, env_list, &shell);
	}
	return (0);
}

	
/* 
 * Boucle principale minishell
 * Affiche le prompt
 * Ajoute la cmd a l'historique		args_handle(list, shell, env_list, input);
 si besoin
 * Exit si CTRL-D
*/
int	is_minishell(t_shell *shell, t_args *env_list, t_args *list, char *user)
{
	char	*input;
	char	*prompt_char;
	char	buf[1024];

	input = NULL;
	prompt_char = NULL;
	if (1 == 1)
	{
		prompt_char = prompt_cmd(shell, user);
		input = readline(prompt_char);
		free(prompt_char);
		if (input == NULL)
			return (2);
		free(shell->is_pwd);
		free(shell->pwd);
		shell->is_pwd = ft_strdup(getcwd(buf, sizeof(buf)));
		shell->pwd = ft_strdup(getcwd(buf, sizeof(buf)));
		input = check_if_there_is_a_lost_pipe(input);
		if (input != NULL)
		{
			add_history(input);
			shell->input_bis = ft_strdup(input);
			if (! shell->input_bis)
			{
				free(input);
				return (1);
			}
			if (main_bis(input, list, env_list, shell) == 1)
			{
				free_everything(shell, list, env_list);
				return (1);
			}
		}
	}
	while (1)
	{
		prompt_char = prompt_cmd(shell, user);
		input = readline(prompt_char);
		free(prompt_char);
		if (input == NULL)
			return (2);
		input = check_if_there_is_a_lost_pipe(input);
		if (input != NULL)
		{
			if (!(ft_strcmp(shell->input_bis, input) == 0 \
				&& ft_strlen(shell->input_bis) == ft_strlen(input)))
				add_history(input);
			if (shell->input_bis)
				free(shell->input_bis);
			shell->input_bis = ft_strdup(input);
			if (! shell->input_bis)
			{
				free(input);
				return (1);
			}
			if (main_bis(input, list, env_list, shell) == 1)
				return (1);
		}
	}
	return (1);
}
