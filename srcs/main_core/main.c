/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 20:36:50 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/05 19:55:07 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			return (1);
		g_error = 0;
	}
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if (from_input_to_list_of_args(input, &list, &env_list) == 1)
		return (1);
	print_args_list(&list);
	if (list)
	{
		if (is_correct_format(&list) == 0)
			create_sublists(list, shell, &env_list);
	}
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		perror("Failed to restore standard output\n");
	// clear_args_list(&list);
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		perror("Failed to restore standard input\n");
	if (access("tempfile.txt", F_OK != -1))
		unlink("tempfile.txt");
	close(saved_stdout);
	return (0);
}

static void try_to_init_shell(int ac, char **av, t_shell *shell)
{
	(void)ac;
	(void)av;
	g_error = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	shell->list = NULL;
	shell->env_list = NULL;
}

/*
 *(!) Main ne gere plus l'input
*/
int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	ft_bzero(&shell, sizeof shell);
	try_to_init_shell(ac, av, &shell);
	if (set_env(&(shell.env_list), env, &shell) == -1)
		return (-1);
	is_minishell(&shell, shell.env_list, shell.list);
	ft_malloc(0, FREE);
	return (0);
}

/*
 * Boucle principale minishell
 * Affiche le prompt
 * Ajoute la cmd a l'historique		args_handle(list, shell, env_list, input);
 si besoin
 * Exit si CTRL-D
*/
int	is_minishell(t_shell *shell, t_args *env_list, t_args *list)
{
	char	*input;
	char	*prompt_char;
	char	buf[1024];

	input = NULL;
	prompt_char = NULL;
	if (1 == 1)
	{
		prompt_char = prompt_cmd(shell);
		input = readline(prompt_char);
		if (input == NULL)
			ft_exit(list, env_list, shell);
		shell->is_pwd = ft_strdup(getcwd(buf, sizeof(buf)));
		shell->pwd = ft_strdup(getcwd(buf, sizeof(buf)));
		input = check_if_there_is_a_lost_pipe(input);
		if (input != NULL)
		{
			add_history(input);
			shell->input_bis = ft_strdup(input);
			if (!shell->input_bis)
				return (1);
			if (main_bis(input, list, env_list, shell) == 1)
				return (1);
			clear_args_list(&list);
		}
	}
	while (1)
	{
		prompt_char = prompt_cmd(shell);
		input = readline(prompt_char);
		if (input == NULL)
			ft_exit(list, env_list, shell);
		input = check_if_there_is_a_lost_pipe(input);
		if (input != NULL)
		{
			if (!(ft_strcmp(shell->input_bis, input) == 0 \
				&& ft_strlen(shell->input_bis) == ft_strlen(input)))
				add_history(input);
			shell->input_bis = ft_strdup(input);
			if (!shell->input_bis)
				return (1);
			if (main_bis(input, list, env_list, shell) == 1)
				return (1);
			clear_args_list(&list);
		}
	}
	return (1);
}
