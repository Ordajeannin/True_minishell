/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_concerns.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 13:59:01 by asalic            #+#    #+#             */
/*   Updated: 2023/12/18 17:30:47 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Modif de l'env pour cd
 * Parcourt env_list jusqu'a l'element que l'on veut changer.
 * Puis, modifier cette valeur avec new_str
*/
void	change_env_cd(t_args **env_list, char *new_str, char *change_value)
{
	t_args	*current;

	current = *env_list;
	while (current && current->str != NULL)
	{
		if (ft_strcmp(current->str, change_value) == 0 && \
			ft_strlen(current->str) == ft_strlen(change_value))
		{
			current->str = ft_strdup(new_str);
			return ;
		}
		current = current->next;
		if (current == NULL)
			add_env(env_list, new_str);
	}
}

/* 
 * Modif de env pour export
 * Recherche de name_env dans env et modifs.
 * Boucle jusqu'a trouver name_env dans l'env.
 * Puis, modifie sa valeur avec value.
*/
int	change_env_exp(t_args **env_list, char *name_env, char *value)
{
	char	*result;
	char	*tmp;
	char	*current_name;
	t_args	*current;

	tmp = ft_strjoin(name_env, "=");
	if (! tmp)
		return (2);
	if (ft_strcmp(name_env, "SHLVL") == 0 && ft_strlen((const char *)name_env) \
		== 5 && ft_atoi(value) < 0)
		result = ft_strjoin(tmp, "0");
	else
		result = ft_strjoin(tmp, value);
	if (!result)
		return (2);
	current = *env_list;
	while (current && current->next != NULL)
	{
		if (help_cee(&current_name, &current) == 2)
			return (2);
		if (help_cee2(&current_name, &current, &result, &name_env) == 0)
			return (0);
	}
	return (1);
}

/* 
 * Boucle principale d'unset.
 * Cherche une VE et la supprime s'il la trouve.
*/
int	searchin_env(t_args **env_list, t_args *list)
{
	t_args	*current;
	t_args	*temp;
	char	*name_env;
	size_t	len;

	current = *env_list;
	len = ft_strlen(list->next->str);
	while (current && current->next)
	{
		name_env = ft_strdupto_n(current->next->str, '=');
		if (!name_env)
			return (1);
		if (ft_strncmp(list->next->str, name_env, len) == 0 && \
		len == ft_strlen(name_env))
			return (help_s_e(&temp, &current));
		current = current->next;
	}
	return (1);
}

/* 
 * Fonction a l'image de 'env'
 * Affiche l'environnement du shell en entier
 * (Attention : env -i ./minishell doit afficher PWD, SHLVL et _)
*/
int	ft_env(t_args *list, t_args **env_list, t_shell *shell)
{
	t_args	*current;

	update_last_ve(list, env_list);
	current = *env_list;
	if (list->next != NULL)
		return (1);
	while (current != NULL && current->str != NULL)
	{
		// if (ft_strncmp(current->str, "?=", 2) == 0)
		// 	current = current->next;
		// else
		{
			ft_printf("%s\n", current->str);
			current = current->next;
		}
	}
	set_error_nb(0, YES);
	return (0);
	///////awena////////
	if (!change_error(env_list, shell, 0))
		return (1);
}

/* 
 * Initialise liste d'env 
*/
int	set_env(t_args **env_list, char **env, t_shell *shell)
{
	int		i;
	char	*identifier;

	if (*env == NULL)
	{
		set_empty_env(shell, env_list);
		return (0);
	}
	if (handle_env(env, shell) == -1)
		return (-1);
	i = 0;
	while (env[i])
	{
		if (help_set_env(env_list, env, &i, &identifier) == -1)
			return (-1);
		if (help_set_env2(env_list, &shell, &identifier, &i) == -1)
			return (-1);
	}
	add_env(env_list, "?=0");
	return (0);
}

/////////////////////////////////PAUL///////////////////

t_args	*find_a(char *var, t_args *env)
{
	char	*str;

	while (env)
	{
		str = ft_strdupto_n(env->str, '=');
		if (ft_strcmp(str, var) == 0)
			return (env);
		env = env->next;
	}
	printf("vale don't found\n");
	return (NULL);
	// str = ft_strdup(var);
	// if (ft_strchr(var, '='))
	// 	ft_strlcpy(str, var, ft_strchr(var, '=') - var + 1);
	// while (ft_strncmp(env->str, str, ft_strchr(env->str, '=') - env->str)
	// 	&& ft_strncmp(env->str, str, ft_strlen(str)) && env->next)
	// 	env = env->next;
	// if (!strncmp(env->str, str, ft_strchr(env->str, '=') - env->str)
	// 	&& !ft_strncmp(env->str, str, ft_strlen(str)))
	// {	printf("var found\n");
	// 	return (env);
	// }
	// else
	// {
	// 	printf("var not found\n");
	// 	return (NULL);
	// }
}

void	upgrade_shlvl(t_args *env)//ajoute 1 a SHLVL dans l'env lors du lancement
{
	t_args	*shlvl;
	int		shlvl_nmb;
	char	*new_shlvl;

	if (find_a("SHLVL", env))
	{
		shlvl = find_a("SHLVL", env);
		shlvl_nmb = ft_atoi(ft_strchr(shlvl->str, '=') + 1) + 1;
		new_shlvl = ft_itoa(shlvl_nmb);
		shlvl->str = ft_strjoin("SHLVL=", new_shlvl);
	}
	else
		add_env(&env, "SHLVL=1");	
}

t_args	*create_env(t_shell *data, char **envp)//cree l'env (liste chainée) a partir de envp (char **)
{
	size_t	index;
	t_args	*first;
	t_args	*new;

	data->secret_pwd = getcwd(NULL, 0);//impossible de lance le programme depuis un sous-repertoire qui n'existe plus
	data->env_list = ft_malloc(sizeof(t_args), ALLOC);
	data->env_list->next = NULL;
	// *envp = NULL;
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
	add_env(&(data->env_list), "?=0");
	data->env_list->next = NULL;
	upgrade_shlvl(first);
	add_env(&data->env_list, "?=0");
	return (first);
}
