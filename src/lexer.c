/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 21:53:46 by fabi              #+#    #+#             */
/*   Updated: 2024/11/13 20:05:34 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// questa funzione se non trova la matching ending quote dovrebbe dare errore
// Salta la virgoletta iniziale (start = *i + 1)
// Evita il loop infinito (*i = end)
// *i = end + 1; // Sposta l'indice oltre la virgoletta di chiusura
char	*extract_quote(const char *input, size_t *i)
{
	size_t	start;
	size_t	end;
	char	*text;

	start = *i + 1; // Salta la virgoletta iniziale
	end = start;
	while (input[end] && input[end] != input[*i])
		end++;
	if (input[end] != input[*i]) // Mancata chiusura
	{
		fprintf(stderr, "Error: unmatched quote\n");
		*i = end; // Sposta alla fine per evitare loop
		return (NULL);
	}
	text = malloc(end - start + 1);
	if (!text)
	{
		perror("malloc");
		return (NULL);
	}
	strncpy(text, &input[start], end - start);
	text[end - start] = '\0';
	*i = end + 1; // Sposta l'indice oltre la virgoletta di chiusura
	return (text);
}

// Funzione per estrarre una parola dall'input
char	*extract_word(const char *input, size_t *i)
{
	size_t	start;

	start = *i;
	while (input[start] && isspace(input[start]))
		start++;

	size_t end = start;
	while (input[end] && !isspace(input[end]) && input[end] != '|' &&
		   input[end] != '>' && input[end] != '<')
		end++;

	char *word = malloc(end - start + 1);
	if (!word)
	{
		perror("malloc");
		return NULL;
	}
	strncpy(word, &input[start], end - start);
	word[end - start] = '\0';

	*i = end; // Sposta l'indice alla posizione successiva
	return word;
}

char *extract_token(const char *input, size_t *i, t_token_type *type)
{
	if (input[*i] == '"' || input[*i] == '\'')
	{
		*type = TOKEN_TEXT; // Contrassegna come TOKEN_TEXT
		return extract_quote(input, i);
	}
	else
	{
		*type = TOKEN_WORD; // Contrassegna come TOKEN_WORD
		return extract_word(input, i);
	}
}

void token_helper(t_token **tokens, const char *input, size_t *i)
{
	if (input[*i] == '>' || input[*i] == '<')
	{
		char type = input[*i];
		if (input[*i + 1] == type)
		{
			if (type == '>')
				add_token(tokens, TOKEN_APPEND, ">>");
			else
				add_token(tokens, TOKEN_HEREDOC, "<<");
			*i += 2;
		}
		else
		{
			if (type == '>')
				add_token(tokens, TOKEN_REDIRECT_OUT, ">");
			else
				add_token(tokens, TOKEN_REDIRECT_IN, "<");
			(*i)++;
		}
	}
}

// Funzione principale del lexer, input sarà l'argomento del programma. Mi passo la i e la lista per ridurre righe di codice
t_token *lexer(const char *input, size_t i, t_token *tokens)
{
	while (input[i] != '\0')
	{
		if (isspace(input[i])) // Ignora spazi
			i++;
		else if (input[i] == '|')
		{
			add_token(&tokens, TOKEN_PIPE, "|");
			i++;
		}
		else if (input[i] == '>' || input[i] == '<')
		{
			token_helper(&tokens, input, &i);
		}
		else
		{
			t_token_type type;
			char *token = extract_token(input, &i, &type);
			if (token)
			{
				add_token(&tokens, type, token);
				free(token);
			}
		}
	}
	return tokens;
}
