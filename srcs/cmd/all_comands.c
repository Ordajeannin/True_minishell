/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_comands.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 17:18:10 by asalic            #+#    #+#             */
/*   Updated: 2023/08/14 18:10:26 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Debut de all_cmd.
 * Cherche si la commande existe ou non a partir de PATH.
*/
static char	*error_cmd(t_args *arg, t_shell *shell, t_args *list,
	t_args **env_list)
{
	char	*command;

	command = is_path_or_cmd(shell->cmd_paths, arg->str, shell);
	if (command == NULL)
	{
		if (access(list->str, F_OK) == 0)
			return (list->str);
	}
	else if (ft_strncmp(command, "It's env", ft_strlen(command)) == 0)
		ft_env(list, env_list, shell);
	return (command);
}

/* 
 * Execution des commandes dependantes de PATH.
 * Creation d'un sous-processus pour execve.
 * Check si la commande est valide.
 * Check si ses arguments sont valides.
 * Si non, return (1) et arret de l'execution de l'invite de commandes.
*/
int	all_cmd(t_args *arg, t_shell *shell, t_args **list, t_args **env_list)
{
	pid_t	pid_child;
	char	*command;
	int		status;
	char	**env_copy;

	env_copy = dup_double_string(env_list);
	command = error_cmd(arg, shell, *list, env_list);
	if (ft_strncmp(command, "It's env", ft_strlen(command)) == 0)
		return (0);
	loop_args(shell, list);
	pid_child = fork();
	if (pid_child == 0)
		execve(command, shell->input, env_copy);
	else
	{
		waitpid(pid_child, &status, 0);
		if (ft_strcmp("./minishell", command) == 0 && ft_strlen(command) == 11)
			ft_less_shell(shell, env_list);
		if (WEXITSTATUS(status) != 0)
			return (1);
	}
	return (0);
}

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
void	ft_plus_shell(t_shell *shell, t_args **env_list)
{
	unsigned int		nb_shell;

	nb_shell = ft_atoi_evolve(shell->shlvl) + 1;
	shell->shlvl = ft_strjoin("SHLVL=", ft_itoa(nb_shell));
	change_env_exp(env_list, "SHLVL", ft_itoa(nb_shell));
}

void	ft_less_shell(t_shell *shell, t_args **env_list)
{
	unsigned int		nb_shell;

	nb_shell = ft_atoi_evolve(shell->shlvl) - 1;
	shell->shlvl = ft_strjoin("SHLVL=", ft_itoa(nb_shell));
	change_env_exp(env_list, "SHLVL", ft_itoa(nb_shell));
}
