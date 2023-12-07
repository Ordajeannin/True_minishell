/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_comand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 11:34:51 by asalic            #+#    #+#             */
/*   Updated: 2023/12/07 19:31:19 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getcwd()
{
	char 	*cwd;
	int		i;

	i = 0;
	cwd = NULL;
	while (!cwd)
	{
		i += 50;
		cwd = ft_malloc(sizeof(char) * i, ALLOC);
		ft_bzero(cwd, i);
		cwd = getcwd(cwd, i);
	}
	return (cwd);
}


/*
 * Change de repertoire en fonction du buf envoye.
 * Agit reellement comme la commande cd.
*/
int	cd_real_version(char *buf, t_shell *shell, t_args *env_list, t_args *list)
{
	shell->oldpwd = ft_getcwd();
	if (chdir(buf) == -1)
	{
		ft_printf("%s: %s: %s\n", list->str, buf, strerror(errno));
		change_error(&env_list, shell, handle_error(errno -1));
		return (1);
	}
	update_pwd(env_list, shell);
	// else
	// {
	// 	if (!cd_move_and_change(env_list, shell))
	// 		return (1);
	// }
	return (0);
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
		return (help_itp1(env_list, &shell, &dir));
	buf = ft_strdup(list->next->str);
	if (! buf)
		help_itp2(&dir);
	// if (shell->pwd == NULL)
	// {
	// 	if (!cd_move_and_change(env_list, shell))
	// 		return (help_itp2(&dir));
	// }
	help_itp2(&dir);
	return (buf);
}

/*
 * Check les arguments de cd
 * Gere cas d'erreurs premiers
*/
int	check_cd(t_args *list, t_shell *shell, t_args *env_list)
{
	// if (list->next != NULL && list->next->str[0] == '\0')
	// 	return (1);
	if (list->next == NULL || !list->next->str || ft_strncmp(list->next->str, "~",
			ft_strlen(list->next->str)) == 0)
	{
		if (!find_a("HOME", env_list))
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
		buf = ft_strdup(find_a("HOME", env_list)->str + 5);
	else if (ft_strncmp(list->next->str, "..", ft_strlen(list->next->str)) == 0)
		buf = is_two_points(shell, list, env_list);
	else
		buf = ft_strdup(list->next->str);
	if (!buf)
		return (1);
	err = cd_real_version(buf, shell, env_list, list);
	return (err || !change_error(&env_list, shell, 0));
}

///////////////////////////////////PAUL///////////////////////////////////////

// int	check_cd(t_args *list, t_shell *shell, t_args *env_list)
// {
	
// }

// void	ft_cd(t_args *list, t_shell *shell, t_args *env_list)//change le working directory
// {
// 	if (list->next == NULL || !list->next->str || ft_strncmp(list->next->str, "~", 2))
// 	{
// 		if (find_a("HOME", data->env))
// 		{
// 			chdir(find_a("HOME", data->env)->data + 5);
// 			update_pwd(data);
// 		}
// 		else
// 		{
// 			data->error_number = 1;
// 			printf("bash: cd: HOME not set\n");
// 		}
// 	}
// 	if (tab[1] && tab[2])
// 	{
// 		printf("minishell: cd: trop d'arguments");
// 		data->error_number = 1;
// 		return ;
// 	}
// 	if (chdir(tab[1]) != 0)
// 	{
// 		printf("erno :%d\n", errno);
// 		if (errno == 13)
// 		{
// 			printf("minishell: cd: %s: Permission denied\n", tab[1]);
// 			data->error_number = 1;
// 		}
// 		if (errno == 116)
// 		{
// 			printf("bash: cd: %s: No such file or directory\n", tab[1]);
// 			data->error_number = 1;
// 		}
// 	}
// 	else
// 		update_pwd(data);
// }