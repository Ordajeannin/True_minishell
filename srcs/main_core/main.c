/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/03 20:36:50 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 19:07:14 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Suite du main.
 * :WARNING: j'ai uncommented clear_args_list, qui avant devait free.
 * on a du se dire qu'elle ne servait plus a rien, mais elle set qd meme
 * list a NULL, conservons la.
*/
int	main_bis(char *input, t_args *env_list, \
	t_shell *shell)
{
	int		saved_stdout;
	int		saved_stdin;
	int		fitloa_ret;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	fitloa_ret = from_input_to_list_of_args(input, shell, &env_list);
	if (fitloa_ret == 1)
		return (fitloa_ret);
	if (shell->list && fitloa_ret != 2)
	{
		if (is_correct_format(&(shell->list)) == 0)
			create_sublists(shell->list, shell, &env_list);
	}
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		perror("Failed to restore standard output\n");
	shell->list = NULL;
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		perror("Failed to restore standard input\n");
	if (access("tempfile.txt", F_OK != -1))
		unlink("tempfile.txt");
	close(saved_stdout);
	return (0);
}

static void	try_to_init_shell(int ac, char **av, t_shell *shell)
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
