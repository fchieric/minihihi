/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 20:04:44 by fabi              #+#    #+#             */
/*   Updated: 2024/11/14 13:27:27 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// TODO aggiungere libft e implementarne le ft_funzioni
// Funzione per aggiungere un token alla lista
// TODO usare safe malloc
void	add_token(t_token **tokens, t_token_type type, const char *value)
{
	t_token	*new_token;
	t_token	*tmp;

	new_token = safe_malloc(sizeof(t_token));
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	if (*tokens == NULL)
	{
		*tokens = new_token;
	}
	else
	{
		tmp = *tokens;
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = new_token;
	}
}
