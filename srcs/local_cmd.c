/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:13:22 by asalic            #+#    #+#             */
/*   Updated: 2023/07/05 20:05:36 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Change de repertoire.
 * Fonction a l'image de cd.
 * Change aussi PWD dans l'env.
 * (Attention: voir cas cd ~)
*/
void	ft_cd(t_args *list, t_shell *shell, t_args *env_list)
{
	char	*buf;

	if (!list->next)
		buf = getenv("HOME");
	else
		buf = list->next->str;
	if (chdir(buf) == -1)
	{
		ft_printf("bash: %s: %s: %s\n", list->str, list->next->str, \
			strerror(errno));
		return ;
	}
	else
	{
		printf("%s\n", shell->pwd);
		change_env(&env_list, ft_strjoin("OLDPWD=", shell->pwd),
			ft_strjoin("OLDPWD=", shell->oldpwd));
		change_env(&env_list, ft_strjoin("PWD=", getcwd(NULL, 0)),
			ft_strjoin("PWD=", shell->pwd));
		shell->oldpwd = shell->pwd;
		ft_printf("OLDPWD%s\n", shell->oldpwd);
		shell->pwd = getcwd(NULL, 0);
		ft_printf("%s\n", getcwd(NULL, 0));
	}
}

/* 
 * Affiche le repertoire courrant 
 * Fonction a l'image de 'pwd'
*/
void	ft_pwd(void)
{
	if (getcwd(NULL, 0) == NULL)
	{
		return ;
	}
	else
		ft_printf("%s\n", getcwd(NULL, 0));
}

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

/* 
 * Execution des commandes dependantes de PATH 
 * Creation d'un sous-processus pour execve
*/
void	all_cmd(t_args *list, t_shell *shell)
{
	char	*command;
	pid_t	pid_child;
	int		status;

	command = extract_cmd_path(shell->cmd_paths, list->str);
	pid_child = fork();
	if (pid_child == 0)
	{
		execve(command, shell->input, NULL);
		return ;
	}
	else
		waitpid(pid_child, &status, 0);
}
