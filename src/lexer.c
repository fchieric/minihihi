/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 21:53:46 by fabi              #+#    #+#             */
/*   Updated: 2024/11/14 13:56:26 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// questa funzione se non trova la matching ending quote dovrebbe dare errore
// Salta la virgoletta iniziale (start = *i + 1)
// Evita il loop infinito (*i = end)
// *i = end + 1; // Sposta l'indice oltre la virgoletta di chiusura
// if (input[end] != input[*i]) // Mancata chiusura
char	*extract_quote(const char *input, size_t *i, char **envp)
{
	size_t	start;
	size_t	end;
	char	*text;
	char	*temp;

	start = *i + 1; // Salta la virgoletta iniziale
	end = start;
	text = ft_strdup(""); // Inizializza `text` come stringa vuota
	while (input[end] && input[end] != input[*i])
	{
		if (input[*i] == '"' && input[end] == '$')
		{
			temp = ft_strjoin(text, ft_strndup(&input[start], end - start));
			free(text);
			text = expand_env_in_quotes(input, &start, &end, envp, temp);
		}
		else
			end++;
	}
	if (input[end] != input[*i])
		free_exit_str(text, 1, "Error: unmatched quote");
	temp = ft_strjoin(text, ft_strndup(&input[start], end - start));
	free(text);
	*i = end + 1;
	return (temp);
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

char *extract_token(const char *input, size_t *i, t_token_type *type, char **envp)
{
	if (input[*i] == '"' || input[*i] == '\'')
	{
		*type = TOKEN_TEXT;
		return extract_quote(input, i, envp);
	}
	else
	{
		*type = TOKEN_WORD;
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
t_token *lexer(const char *input, size_t i, t_token *tokens, char **envp)
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
			char *token = extract_token(input, &i, &type, envp);
			if (token)
			{
				add_token(&tokens, type, token);
				free(token);
			}
		}
	}
	return (tokens);
}
