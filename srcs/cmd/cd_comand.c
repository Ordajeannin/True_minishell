/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_comand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 11:34:51 by asalic            #+#    #+#             */
/*   Updated: 2023/10/05 19:24:22 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Change de repertoire en fonction du buf envoye.
 * Agit reellement comme la commande cd.
*/
int	cd_real_version(char *buf, t_shell *shell, t_args *env_list, t_args *list)
{
	if (chdir(buf) == -1)
	{
		ft_printf("%s: %s: %s\n", list->str, buf, strerror(errno));
		change_error(&env_list, shell, handle_error(errno -1));
		return (1);
	}
	else
	{
		if (!cd_move_and_change(env_list, shell))
			return (1);
	}
	return (0);
}

/*
 * Permet de normer is_two_point
*/
static char *help_itp1(t_args *env_list, t_shell **shell, DIR **dir,
		char **temp)
{
	ft_printf("cd : No such file of directory\n");
	change_error(&env_list, *shell, 0);
	closedir(*dir);
	free(*temp);
	return (NULL);
}

/*
 * Permet de normer is_two_points
 * fonction recurrente dans le code
*/
static char	*help_itp2(DIR **dir, char **temp)
{
	free(*temp);
	closedir(*dir);
	return (NULL);
}

/*
 * Cas ou cd.., suite de la commande cd principale
 * Fonction normee, conservation du code commente au cas ou
*/
static char	*is_two_points(t_shell *shell, t_args *list, t_args *env_list)
{
	DIR		*dir;
	char	*temp;
	char	*buf;

	temp = from_end_to_char(shell->is_pwd, '/');
	if (!temp)
		return (NULL);
	dir = opendir(temp);
	if (dir == NULL)
		return (help_itp1(env_list, &shell, &dir, &temp));
//	{
//		buf = NULL; pourquoi set a NULL alors qu on return?
//		ft_printf("cd : No such file or directory\n");
//		change_error(&env_list, shell, 0);
//		closedir(dir);
//		free(temp);
//		return (NULL);
//	}
	buf = ft_strdup(list->next->str);
	if (! buf)
		help_itp2(&dir, &temp);
//	{
//		free(temp);
//		closedir(dir);
//		return (NULL);
//	}
	if (shell->pwd == NULL)
	{
		if (!cd_move_and_change(env_list, shell))
			return(help_itp2(&dir, &temp));
//		{
//			free(temp);
//			closedir(dir);
//			return (NULL);
//		}
	}
	help_itp2(&dir, &temp);
//	free(temp);
//	closedir(dir);
	return (buf);
}

/* 
 * Check les arguments de cd
 * Gere cas d'erreurs premiers
*/
int	check_cd(t_args *list, t_shell *shell, t_args *env_list)
{
	if (list->next != NULL && list->next->str[0] == '\0')
		return (1);
	if (list->next == NULL || ft_strncmp(list->next->str, "~",
			ft_strlen(list->next->str)) == 0)
	{
		if (shell->home == NULL)
		{
			ft_printf("%s: 'HOME' not defined\n", list->str);
			change_error(&env_list, shell, 1);
			return (1);
		}
		return (2);
	}
	else if (list->next->next && list->next->next->token != TOKEN_AND
		&& list->next->next->token != TOKEN_OR)
	{
		ft_printf("%s: too many arguments\n", list->str);
		change_error(&env_list, shell, 1);
		return (1);
	}
	return (0);
}

/* 
 * Check si cd .. || cd ~ || autre cd
 * Agit en fonction des cas speciaux, un peu comme une gestionnaire
 * d'arguments pour cd specifiquement.
*/
int	ft_cd(t_args *list, t_shell *shell, t_args *env_list)
{
	char	*buf;
	int		cod;
	int		err;

	cod = check_cd(list, shell, env_list);
	if (cod == 1)
		return (1);
	if (cod == 2)
		buf = ft_strdup(shell->home);
	else if (ft_strncmp(list->next->str, "..", ft_strlen(list->next->str)) == 0)
		buf = is_two_points(shell, list, env_list);
	else
		buf = ft_strdup(list->next->str);
	if (!buf)
		return (1);
	err = cd_real_version(buf, shell, env_list, list);
	free(buf);
	return (err || !change_error(&env_list, shell, 0));
}
