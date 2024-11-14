/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 21:53:46 by fabi              #+#    #+#             */
/*   Updated: 2024/11/14 11:59:31 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// questa funzione se non trova la matching ending quote dovrebbe dare errore
// Salta la virgoletta iniziale (start = *i + 1)
// Evita il loop infinito (*i = end)
// *i = end + 1; // Sposta l'indice oltre la virgoletta di chiusura
// if (input[end] != input[*i]) // Mancata chiusura
char	*extract_quote(const char *input, size_t *i)
{
	size_t	start;
	size_t	end;
	char	*text;

	text = NULL;
	start = *i + 1;
	end = start;
	while (input[end] && input[end] != input[*i])
		end++;
	if (input[end] != input[*i])
	{
		*i = end;
		free_exit_str(text, 1, "Error: unmatched quote");
	}
	text = safe_malloc(end - start + 1);
	ft_strncpy(text, &input[start], end - start);
	text[end - start] = '\0';
	*i = end + 1;
	return (text);
}

// Funzione per estrarre una parola dall'input
// *i = end; // Sposta l'indice alla posizione successiva
char	*extract_word(const char *input, size_t *i)
{
	size_t	start;
	size_t	end;
	char	*word;

	start = *i;
	while (input[start] && ft_isspace(input[start]))
		start++;
	end = start;
	while (input[end] && !ft_isspace(input[end]) && input[end] != '|' &&
			input[end] != '>' && input[end] != '<')
		end++;
	if (end - start == 3 && ft_strncmp(&input[start], "EOF", 3) == 0)
	{
		*i = end;
		return (ft_strdup("EOF"));
	}
	word = safe_malloc(end - start + 1);
	ft_strncpy(word, &input[start], end - start);
	word[end - start] = '\0';
	*i = end;
	return word;
}

char *extract_token(const char *input, size_t *i, t_token_type *type)
{
	if (input[*i] == '"' || input[*i] == '\'')
	{
		*type = TOKEN_TEXT;
		return extract_quote(input, i);
	}
	else
	{
		char *token = extract_word(input, i);
		if (token && ft_strncmp(token, "EOF", 3) == 0)
		{
			*type = TOKEN_EOF;
			return token;
		}
		*type = TOKEN_WORD;
		return token;
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
	t_token_type type;
	
	while (input[i] != '\0')
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '|')
		{
			add_token(&tokens, TOKEN_PIPE, "|");
			i++;
		}
		else if (input[i] == '>' || input[i] == '<')
			token_helper(&tokens, input, &i);
		else
		{
			char *token = extract_token(input, &i, &type);
			if (token)
			{
				add_token(&tokens, type, token);
				free(token);
			}
		}
	}
	return (tokens);
}
