/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_concerns.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 13:59:01 by asalic            #+#    #+#             */
/*   Updated: 2023/07/10 12:11:53 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Modification de l'env
 * Parcourt env_list jusqu'a l'element que l'on veut changer.
 * Puis, modifier cette valeur avec new_str
*/
void	change_env(t_args **env_list, char *new_str, char *change_value)
{
	t_args	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strncmp(current->str, change_value, ft_strlen(current->str))
			== 0)
		{
			current->str = new_str;
			return ;
		}
		current = current->next;
	}
	return ;
}

/* 
 * Fonction a l'image de 'env'
 * Affiche l'environnement du shell en entier
 * (Attention : env -i ./minishell doit afficher PWD, SHLVL et _)
*/
void	ft_env(t_args *list, t_args *env_list)
{
	if (list->next != NULL)
		ft_printf("bash: %s: %s: %s\n", list->str, list->next->str, \
			strerror(errno));
	while (env_list != NULL)
	{
		ft_printf("%s\n", env_list->str);
		env_list = env_list->next;
	}
}

/* PROTECTION DE MALLOC! 
 * Adaptation de list pour execve
 * Boucle qui remplit shell->input
*/
void	loop_args(t_shell *shell, t_args **list)
{
	t_args	*current;
	int		len;
	int		len_list;
	int		i;

	len = 0;
	len_list = 0;
	current = *list;
	while (current)
	{
		len_list ++;
		current = current->next;
	}
	shell->input = malloc((len_list +1) * sizeof(char *));
	current = *list;
	i = 0;
	while (current)
	{
		len = ft_strlen(current->str);
		shell->input[i] = malloc((len +1) * sizeof(char));
		shell->input[i++] = current->str;
		current = current->next;
	}
	shell->input[i] = NULL;
}

/* 
 * Execution des commandes dependantes de PATH 
 * Creation d'un sous-processus pour execve
*/
void	all_cmd(t_args *arg, t_shell *shell, t_args **list)
{
	char	*command;
	pid_t	pid_child;
	int		status;

	command = extract_cmd_path(shell->cmd_paths, arg->str);
	loop_args(shell, list);
	pid_child = fork();
	if (pid_child == 0)
	{
		ft_printf("\n");
		execve(command, shell->input, NULL);
		return ;
	}
	else
		waitpid(pid_child, &status, 0);
}

/* 
 * Changement des valeurs de la structure t_shell.
 * S'effectue apres unset (et export aussi !!)
*/
void	shell_change(t_shell *shell, t_args *list)
{
	int		len;

	len = ft_strlen(list->next->str);
	if (ft_strncmp(list->next->str, "HOME", len) == 0)
		shell->home = NULL;
	else if (ft_strncmp(list->next->str, "PWD", len) == 0)
		shell->pwd = NULL;
	else if (ft_strncmp(list->next->str, "OLDPWD", len) == 0)
		shell->oldpwd = NULL;
	else if (ft_strncmp(list->next->str, "USER", len) == 0)
		shell->user = NULL;
	else if (ft_strncmp(list->next->str, "SHELL", len) == 0)
		shell->shell = NULL;
	else if (ft_strncmp(list->next->str, "PATH", len) == 0)
		shell->path = NULL;
	else if (ft_strncmp(list->next->str, "LANG", len) == 0)
		shell->lang = NULL;
	else if (ft_strncmp(list->next->str, "TERM", len) == 0)
		shell->term = NULL;
	else if (ft_strncmp(list->next->str, "HOSTNAME", len) == 0)
		shell->hostname = NULL;
	else if (ft_strncmp(list->next->str, "SHLVL", len) == 0)
		shell->shlvl = NULL;
	return ;
}
