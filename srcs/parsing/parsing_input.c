/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 15:19:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/07/07 18:31:07 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void delimit_to_token(char *str, t_args **list)
{
	char *quote_end;

	quote_end = NULL;
    if (ft_strcmp(str, " ") != 0 && ft_strcmp(str, "") != 0)
    {
        if (str[0] == '\'' || str[0] == '\"')
        {
            quote_end = ft_strchr(str + 1, str[0]);
            if (quote_end == NULL)
				ft_printf("Error : quotes aren't ended\n");
            else
            {
                str++;
                *quote_end = '\0';
                add_arg(list, str, 0);
            }
        }
        else if (ft_strcmp(str, "<") == 0 || ft_strcmp(str, ">") == 0)
            add_arg(list, str, 0);
        else
            add_arg(list, str, 1);
    }
    else
        return;
}

char *ft_strtok(char *input, char **delim, t_args **list)
{
    static char *last_token = NULL;
    char *token_end;
	char *quote_end;

    if (input != NULL)
        last_token = input;
    else
    {
        if (last_token == NULL)
            return NULL;
        input = last_token;
    }
    if (*input == '\0')
    {
        last_token = NULL;
        return NULL;
    }
    while (*input == ' ' || *input == '\t')
        input++;
    if (*input == '\'' || *input == '\"')
    {
        quote_end = ft_strchr(input + 1, *input);
        if (quote_end == NULL)
            return NULL;
        token_end = quote_end + 1;
    }
    else
    {
        token_end = ft_strpbrk(input, " \t<>\"");
        if (token_end == NULL || *token_end == '\0')
            token_end = input + ft_strlen(input);
    }
    if (token_end != input)
    {
        *token_end = '\0';
        delimit_to_token(input, list);
        last_token = token_end + 1;
    }
    else
        last_token = token_end;
    return input;
}
