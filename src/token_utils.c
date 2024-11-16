/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 18:45:11 by fabi              #+#    #+#             */
/*   Updated: 2024/11/16 18:42:54 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* Aggiunge un nuovo token alla lista di token
* Se tokens è NULL, crea il primo token
* Altrimenti aggiunge il nuovo token alla fine della lista
*/
void	add_token(t_token **tokens, t_token_type type, const char *value)
{
	t_token	*new_token;
	t_token	*last;

	new_token = safe_malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	if (!new_token->value)
	{
		free(new_token);
		return ;
	}
	if (!*tokens)
		*tokens = new_token;
	else
	{
		last = *tokens;
		while (last->next)
			last = last->next;
		last->next = new_token;
	}
}

/*
* Libera un singolo token e il suo contenuto
*/
void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
		free(token->value);
	free(token);
}

/*
* Libera l'intera lista di token
*/
void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free_token(tokens);
		tokens = next;
	}
}

