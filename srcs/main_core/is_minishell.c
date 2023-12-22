/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_minishell.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 18:37:22 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 19:10:02 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_minishell_part2(t_shell *shell)
{
	char	*input;

	input = NULL;
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
		shell->list = NULL;
	}
	return (0);
}

int	is_minishell_part1(t_shell *shell)
{
	char	*input;

	input = NULL;
	input = ft_readline(prompt_cmd(shell), YES);
	if (input == NULL)
		ft_exit(shell->list, shell->env_list, shell);
	input = check_if_there_is_a_lost_pipe(input);
	if (input != NULL)
	{
		add_history(input);
		shell->input_bis = ft_strdup(input);
		if (!shell->input_bis)
			return (1);
		if (main_bis(input, shell->env_list, shell) == 1)
			return (1);
		shell->list = NULL;
	}
	return (0);
}

int	is_minishell(t_shell *shell)
{
	if (is_minishell_part1(shell) == 1)
		return (1);
	while (1)
	{
		if (is_minishell_part2(shell) == 1)
			return (1);
	}
	return (1);
}
