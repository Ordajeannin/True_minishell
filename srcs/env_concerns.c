/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_concerns.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 13:59:01 by asalic            #+#    #+#             */
/*   Updated: 2023/07/10 18:27:39 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Modification de l'env
 * Parcourt env_list jusqu'a l'element que l'on veut changer.
 * Puis, modifier cette valeur avec new_str
*/
int	change_env(t_args **env_list, char *new_str, char *change_value)
{
	t_args	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strncmp(current->str, change_value, ft_strlen(current->str))
			== 0)
		{
			current->str = NULL;
			current->str = new_str;
			return (1);
		}
		current = current->next;
	}
	return (0);
}

/* 
 * Fonction a l'image de 'env'
 * Affiche l'environnement du shell en entier
 * (Attention : env -i ./minishell doit afficher PWD, SHLVL et _)
*/
void	ft_env(t_args *list, t_args **env_list)
{
	t_args *current;

	current = *env_list;
	if (list->next != NULL)
		ft_printf("bash: %s: %s: %s\n", list->str, list->next->str, \
			strerror(errno));
	while (current != NULL)
	{
		ft_printf("%s\n", current->str);
		current = current->next;
	}
}

/* 
 * PROTECTION DE MALLOC! 
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
void	shell_change(t_shell *shell, char *str, char *value)
{
	int		len;

	len = ft_strlen(str);
	if (ft_strncmp(str, "HOME", len) == 0)
		shell->home = value;
	else if (ft_strncmp(str, "PWD", len) == 0)
		shell->pwd = value;
	else if (ft_strncmp(str, "OLDPWD", len) == 0)
		shell->oldpwd = value;
	else if (ft_strncmp(str, "USER", len) == 0)
		shell->user = value;
	else if (ft_strncmp(str, "SHELL", len) == 0)
		shell->shell = value;
	else if (ft_strncmp(str, "PATH", len) == 0)
		shell->path = value;
	else if (ft_strncmp(str, "LANG", len) == 0)
		shell->lang = value;
	else if (ft_strncmp(str, "TERM", len) == 0)
		shell->term = value;
	else if (ft_strncmp(str, "HOSTNAME", len) == 0)
		shell->hostname = value;
	else if (ft_strncmp(str, "SHLVL", len) == 0)
		shell->shlvl = value;
	return ;
}
