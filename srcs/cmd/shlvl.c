/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:03:17 by asalic            #+#    #+#             */
/*   Updated: 2023/12/19 17:02:11 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 Evolution de ft_atoi
 Cherche dans une string un enchainement de nombre et le transforme en nombre
*/
static unsigned int	ft_atoi_evolve(char	*str)
{
	unsigned int	i;
	unsigned int	result;

	i = 0;
	while (str[i] < '0' || str[i] > '9')
		i++;
	result = 0;
	while (str[i] >= '0' && str[i] <= '9')
		result = (result * 10) + (str[i++] - '0');
	return (result);
}

/* 
 Incremente la VE SHLVL
 Lorsque un ./minishell est lance dans un ./minishell -> SHLVL +1
*/
int	ft_plus_shell(t_shell *shell, t_args **env_list)
{
	unsigned int	nb_shell;
	char			*len_shell;

	nb_shell = ft_atoi_evolve(shell->shlvl) + 1;
	len_shell = ft_itoa(nb_shell);
	if (!len_shell)
		return (1);
	shell->shlvl = ft_strdup(len_shell);
	if (!shell->shlvl)
		return (1);
	if (change_env_exp(env_list, "SHLVL", len_shell) == 2)
		return (1);
	return (0);
}

// static int	shell_change_path(t_shell *shell, char *value)
// {
// 	shell->path = NULL;
// 	shell->path = ft_strdup(value);
// 	ft_printf("value of path : %s\n", shell->path);
// 	if (!shell->path)
// 		return (1);
// 	ft_printf("value of path : %s\n", shell->path);
// 	if (shell->path != NULL && ft_strlen(shell->path) >= 5)
// 	{
// 		shell->cmd_paths = ft_split(shell->path + 5, ':');
// 		if (! shell->cmd_paths)
// 			return (-1);
// 	}
// 	return (0);
// }

/* 
 * Changement des valeurs de la structure t_shell.
 * S'effectue apres unset (et export aussi !!)
*/
// void	shell_change(t_shell *shell, char *str, char *value)
// {
// 	int		len;

// 	len = ft_strlen(str);
// 	if (ft_strncmp(str, "HOME", len) == 0)
// 		shell->home = ft_strdup(value);
// 	else if (ft_strncmp(str, "OLDPWD", len) == 0)
// 		shell->oldpwd = ft_strdup(value);
// 	else if (ft_strncmp(str, "PWD", len) == 0)
// 		shell->pwd = ft_strdup(value);
// 	else if (ft_strncmp(str, "PATH", len) == 0)
// 		shell_change_path(shell, value);
// 	else if (ft_strncmp(str, "SHLVL", len) == 0)
// 		shell->shlvl = ft_strdup(value);
// }
