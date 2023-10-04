/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 15:09:25 by asalic            #+#    #+#             */
/*   Updated: 2023/10/04 17:18:42 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_shell_var(t_shell *shell)
{
	int	i;

	i = 0;
	if (shell->is_oldpwd)
		free(shell->is_oldpwd);
	if (shell->is_pwd)
		free(shell->is_pwd);
	if (shell->oldpwd)
		free(shell->oldpwd);
	if (shell->pwd)
		free(shell->pwd);
	if (shell->home)
		free(shell->home);
	if (shell->path)
		free(shell->path);
	if (shell->input_bis)
		free(shell->input_bis);
	if (shell->shlvl)
		free(shell->shlvl);
	if (shell->cmd_paths)
	{
		while (shell->cmd_paths[i])
			free(shell->cmd_paths[i++]);
		free(shell->cmd_paths);
	}
	i = 0;
	if (shell->input)
	{
		while (shell->input[i])
			free(shell->input[i++]);
		free(shell->input);
	}
}

void	free_everything(t_shell *shell, t_args *list, t_args *env_list)
{
	free_shell_var(shell);
	if (env_list)
		clear_args_list(&env_list);
	rl_clear_history();
	clear_args_list(&list);
}

/*
 * Program stop.
 * Fonction a l'image de 'exit' 
 * Quitte le programme proprement avec free.
 * :warning:
 * wait(100) supprime de la premiere ligne pour la norme
*/
int	ft_exit(t_args *list, t_args *env_list, t_shell *shell)
{
	int	code_err;

	code_err = 0;
	(void)shell;
	ft_printf("exit\n");
	if (list && list->next && is_numeric(list->next->str) == 1)
	{
		ft_printf("%s: %s: numeric argument required\n", list->str,
			list->next->str);
		code_err = 2;
	}
	else if (list && list->next && list->next->next)
	{
		ft_printf("%s: too many arguments\n", list->str);
		change_error(&env_list, shell, 1);
		return (1);
	}
	else if (list && list->next)
		code_err = ft_atoi(list->next->str) % 256;
	else if (shell->error == 0 && g_error != 0)
		code_err = g_error;
	else
		code_err = shell->error;
	free_everything(shell, list, env_list);
	exit(code_err);
}
