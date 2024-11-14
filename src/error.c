/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:22:10 by fabi              #+#    #+#             */
/*   Updated: 2024/11/14 11:41:41 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


void free_exit_list(t_token *tokens, int exit_status, char *message)
{
	t_token	*tmp;

	printf("%s\n", message);
	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
	//g_status = exit_status;
	exit(exit_status);
}

void	free_exit_str(char *str, int exit_status, char *message)
{
	t_token	*tmp;
	int		i;

	printf("%s\n", message);
	while (str[i] != '\0')
	{
		free(&str[i]);
		i++;
	}
	//g_status = exit_status;
	exit(exit_status);
}
