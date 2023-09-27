/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:03:17 by asalic            #+#    #+#             */
/*   Updated: 2023/09/27 11:01:06 by asalic           ###   ########.fr       */
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
	shell->shlvl = ft_strjoin("SHLVL=", len_shell);
	if (change_env_exp(env_list, "SHLVL", len_shell) == 2)
	{
		free(len_shell);
		return (1);
	}
	free(len_shell);
	return (0);
}

static void	shell_change_path(t_shell *shell, char *value)
{
	int		i;

	i = 0;
	shell->path = value;
	if (shell->path == NULL)
	{
		while (shell->cmd_paths[i])
			shell->cmd_paths[i++] = NULL;
		shell->cmd_paths = NULL;
	}
	else
		shell->cmd_paths = ft_split(shell->path + 5, ':');
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
	else if (ft_strncmp(str, "OLDPWD", len) == 0)
		shell->oldpwd = value;
	else if (ft_strncmp(str, "PWD", len) == 0)
		shell->pwd = value;
	else if (ft_strncmp(str, "USER", len) == 0)
		shell->user = value;
	else if (ft_strncmp(str, "SHELL", len) == 0)
		shell->shell = value;
	else if (ft_strncmp(str, "PATH", len) == 0)
		shell_change_path(shell, value);
	if (ft_strncmp(str, "LANG", len) == 0)
		shell->lang = value;
	else if (ft_strncmp(str, "TERM", len) == 0)
		shell->term = value;
	else if (ft_strncmp(str, "HOSTNAME", len) == 0)
		shell->hostname = value;
	else if (ft_strncmp(str, "SHLVL", len) == 0)
		shell->shlvl = value;
	return ;
}
