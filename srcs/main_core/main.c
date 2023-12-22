/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 20:36:50 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 17:01:14 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Suite du main.
 * :WARNING: j'ai uncommented clear_args_list, qui avant devait free.
 * on a du se dire qu'elle ne servait plus a rien, mais elle set qd meme
 * list a NULL, conservons la.
*/
static int	main_bis(char *input, t_args *env_list, \
	t_shell *shell)
{
	int		saved_stdout;
	int		saved_stdin;
	int		fitloa_ret;

	// if (g_error != 0)//sert a quoi ?
	// {
	// 	if (!change_error(&env_list, shell, g_error))
	// 		return (1);
	// 	g_error = 0;
	// }
	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	// if (from_input_to_list_of_args(input, shell, &env_list) == 1)
	// 	return (1);
//	printf("____________ MAIN LIST ____________\n");
//	print_args_list(&(shell->list));
//	printf("___________________________________\n");
	fitloa_ret = from_input_to_list_of_args(input, shell, &env_list);
//	if (fitloa_ret == 2)
//		return (0);
	if (fitloa_ret == 1)
		return (fitloa_ret);
	// if (from_input_to_list_of_args(input, &list, &env_list) == 1)
	// 	return (1);
	// print_args_list(&list);
	if (shell->list && fitloa_ret != 2)
	{
		if (is_correct_format(&(shell->list)) == 0)
			create_sublists(shell->list, shell, &env_list);
	}
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		perror("Failed to restore standard output\n");
	clear_args_list(&(shell->list));
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
	shell.env_list = create_env(&shell, env);
	// if (set_env(&(shell.env_list), env, &shell) == -1)
	// 	return (-1);
	is_minishell(&shell);
	ft_readline(NULL, NO);
	ft_malloc(0, FREE);
	return (0);
}

void	free_readline(t_args *read_ret)
{
	while (read_ret)
	{
		free(read_ret->str);
		read_ret = read_ret->next;
	}
}

char	*ft_readline(char *prompt, int add)
{
	static t_args	read_ret;
	t_args			*index;
	t_args			*new_line;

	if (!add)
	{
		free_readline(&read_ret);
		return (NULL);
	}
	index = &read_ret;
	new_line = ft_malloc(sizeof(t_args), ALLOC);
	new_line->str = readline(prompt);
	new_line->next = NULL;
	while (index->next)
		index = index->next;
	index->next = new_line;
	return (new_line->str);
}


/*
 * Boucle principale minishell
 * Affiche le prompt
 * Ajoute la cmd a l'historique		args_handle(list, shell, env_list, input);
 si besoin
 * Exit si CTRL-D
*/
int	is_minishell(t_shell *shell)
{
	char	*input;
	// char	*prompt_char;
	char	buf[1024];
	// int		mb_ret;

	input = NULL;
	// prompt_char = NULL;
	if (1 == 1)
	{
		// prompt_char = prompt_cmd(shell);
		input = ft_readline(prompt_cmd(shell), YES);
		if (input == NULL)
			ft_exit(shell->list, shell->env_list, shell);
		shell->is_pwd = ft_strdup(getcwd(buf, sizeof(buf)));
		shell->pwd = ft_strdup(getcwd(buf, sizeof(buf)));
		input = check_if_there_is_a_lost_pipe(input);
		if (input != NULL)
		{
			add_history(input);
			shell->input_bis = ft_strdup(input);
			if (!shell->input_bis)
				return (1);
			if (main_bis(input, shell->env_list, shell) == 1)
				return (1);
			clear_args_list(&(shell->list));
		}
	}
	while (1)
	{
		// prompt_char = prompt_cmd(shell);
		input = ft_readline(prompt_cmd(shell), YES);
		if (input == NULL)
			ft_exit(shell->list, shell->env_list, shell);
		input = check_if_there_is_a_lost_pipe(input);
		if (input != NULL)
		{
			if (!(ft_strcmp(shell->input_bis, input) == 0 \
				&& ft_strlen(shell->input_bis) == ft_strlen(input)))
				add_history(input);
			shell->input_bis = ft_strdup(input);
			if (!shell->input_bis)
				return (1);
			if (main_bis(input, shell->env_list, shell) == 1)
				return (1);
			clear_args_list(&(shell->list));
		}
	}
	return (1);
}
