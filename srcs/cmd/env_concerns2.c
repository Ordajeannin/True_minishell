/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_concerns2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/22 18:29:45 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/22 18:32:13 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_data(t_shell *data)
{
	data->secret_pwd = ft_getcwd();
	data->env_list = ft_malloc(sizeof(t_args), ALLOC);
	data->env_list->next = NULL;
}

//cree l'env (liste chainée) a partir de envp (char **)
//impossible de lancer le programme depuis un sous-repertoire qui n'existe plus
t_args	*create_env(t_shell *data, char **envp)
{
	size_t	index;
	t_args	*first;
	t_args	*new;

	set_data(data);
	if (*envp == NULL)
	{
		data->env_list->str = ft_strdup("");
		return (data->env_list);
	}
	first = data->env_list;
	data->env_list->str = ft_strdup(envp[0]);
	index = 1;
	while (envp[index])
	{
		new = ft_malloc(sizeof(t_args), ALLOC);
		new->str = ft_strdup(envp[index]);
		data->env_list->next = new;
		data->env_list = new;
		index ++;
	}
	data->env_list->next = NULL;
	upgrade_shlvl(first);
	return (first);
}
