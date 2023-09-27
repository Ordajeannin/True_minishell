/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lsv_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/27 16:50:15 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/27 16:51:45 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
static void	delete_last_node(t_args **list)
{
	t_args	*current;
	t_args	*prev;

//	if (*list == NULL || (*list)->next == NULL)
//	{
//		free(*list);
//		*list = NULL;
//		return ;
//	}
	if ((*list)->token == TOKEN_PIPE && (*list)->next == NULL)
	{
		free(*list);
		*list = NULL;
		return ;
	}
	else if ((*list)->next == NULL)
		return ;
	current = *list;
	prev = NULL;
	while (current->next != NULL)
	{
		prev = current;
		current = current->next;
	}
	if (current->token == TOKEN_PIPE)
	{
		prev->next = NULL;
		free(current);
	}
}
*/

/*
static void	execute_command(t_args *first_cmd, t_args *scd_cmd, int pipefd[2], \
	t_shell *shell, t_args **env_list, char *input)
{
	pid_t	pid1;
	pid_t	pid2;

	delete_last_node(&first_cmd);
	delete_last_node(&scd_cmd);
	printf("------------LIST 1----------------\n");
	print_args_list(&first_cmd);
	printf("------------LIST 2----------------\n");
	print_args_list(&scd_cmd);
	pid1 = fork();
	if (pid1 == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		args_handle(first_cmd, shell, env_list, input);
		exit(EXIT_SUCCESS);
	}
	else if (pid1 < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		args_handle(scd_cmd, shell, env_list, input);
		exit(EXIT_SUCCESS);
	}
	else if (pid2 < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}
*/

/*
void	create_sublists(t_args *list, t_shell *shell, t_args **env_list, \
		char *input)
{
	t_args		*current;
	t_args		*start;
	t_args		*second_list;
	t_args_list	*stock;
	int		pipefd[2];
	int		flag;

	current = list;
	start = list;
	second_list = NULL;
	flag = 0;
	stock = stock_sublist(&list);
	print_sublists(stock);
	printf("------------LIST 0----------------\n");
	print_args_list(&list);
	while (current)
	{
		if (current->token == TOKEN_PIPE)
		{
			if (pipe(pipefd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			printf("debut de la seconde liste : %s\n", current->next->str);
			second_list = current->next;
			current->next = NULL;
			execute_command(start, second_list, pipefd, shell, env_list, input);
			start = second_list;
			flag = 1;
		}
		current = current->next;
	}
	if (flag == 0)
		args_handle(start, shell, env_list, input);
}
*/

/*
char	*check_if_there_is_a_lost_pipe(char *input)
{
	char	*result;
	char	*combined;
	int		lenght;

	result = NULL;
	combined = NULL;
	lenght = ft_strlen(input);
	printf("input : %s\n", input);
	if (lenght > 0 && input[lenght - 1] == '|')
	{
		result = readline("> ");
		combined = (char *)malloc(lenght + ft_strlen(result) + 1);
		if (!combined)
		{
			perror("malloc");
			free(result);
			return (NULL);
		}
		ft_strcpy(combined, input);
		ft_strcat(combined, result);
		free(result);
		return (combined);
	}
	else
		return (input);
}
*/
