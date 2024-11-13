/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 21:53:46 by fabi              #+#    #+#             */
/*   Updated: 2024/11/13 16:13:02 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// TODO aggiungere libft e implementarne le ft_funzioni

// questa funzione se non trova la matching ending quote dovrebbe dare errore
char *extract_quote(const char *input, size_t *i)
{
    size_t start = *i;
    size_t length = 0;
    char quote = input[start];
    start++;
    size_t end = start;
    while (input[end] && input[end] != quote)
    {
        end++;
    }
    if (input[end] != quote)
    {
        // cambiare questa e mettere la nostar gestione degli errori
        fprintf(stderr, "Error: unmatched quote\n");
        return NULL;
    }
    length = end - start;
    char *word = malloc(length + 1);
    if (!word)
    {
        perror("malloc");
        return NULL;
    }
    strncpy(word, &input[start], length);
    word[length] = '\0';
    *i = end;
    return word;
}

// Funzione per aggiungere un token alla lista
void add_token(t_token **tokens, t_token_type type, const char *value)
{
    t_token *new_token = malloc(sizeof(t_token)); // TODO usare safe malloc
    if (!new_token)
    {
        perror("malloc");
        return;
    }
    new_token->type = type;
    new_token->value = strdup(value);
    new_token->next = NULL;
    if (*tokens == NULL)
    {
        *tokens = new_token;
    }
    else
    {
        t_token *tmp = *tokens;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = new_token;
    }
}

// Funzione per estrarre una parola dall'input
char *extract_word(const char *input, size_t *i)
{
    size_t start = *i;
    size_t length = 0;

    // Ignora spazi iniziali
    while (input[start] && isspace(input[start]))
    {
        start++;
    }
    // Trova la fine della parola
    size_t end = start;
    while (input[end] && !isspace(input[end]) && input[end] != '|' &&
           input[end] != '>' && input[end] != '<')
    {
        end++;
    }
    // Calcola la lunghezza della parola
    length = end - start;
    // Copia la parola in una nuova stringa
    char *word = malloc(length + 1);
    if (!word)
    {
        perror("malloc"); // TODO migliorare la gestione degli errori
        return NULL;
    }
    strncpy(word, &input[start], length);
    word[length] = '\0';
    // Aggiorna l'indice del chiamante
    *i = end - 1;
    return word;
}

// Funzione principale del lexer
t_token *lexer(const char *input) // input sarà l'argomento del programma
{
    t_token *tokens = NULL; // Lista collegata di token
    size_t i = 0;

    while (input[i] != '\0')
    {
        if (isspace(input[i])) // Ignora spazi
        {
            i++;
        }
        else if (input[i] == '|')
        {
            // Token PIPE
            add_token(&tokens, TOKEN_PIPE, "|");
            i++;
        }
        else if (input[i] == '>')
        {
            // Token REDIRECT_OUT o APPEND
            if (input[i + 1] == '>')
            {
                add_token(&tokens, TOKEN_APPEND, ">>");
                i += 2;
            }
            else
            {
                add_token(&tokens, TOKEN_REDIRECT_OUT, ">");
                i++;
            }
        }
        else if (input[i] == '<')
        {
            // Token REDIRECT_IN o HEREDOC
            if (input[i + 1] == '<')
            {
                add_token(&tokens, TOKEN_HEREDOC, "<<");
                i += 2;
            }
            else
            {
                add_token(&tokens, TOKEN_REDIRECT_IN, "<");
                i++;
            }
        }
        else if (input[i] == '"' || input[i] == '\'')
        {
            // Token QUOTE
            char *quote = extract_quote(input, &i);
            if (quote)
            {
                add_token(&tokens, TOKEN_QUOTE, quote);
                free(quote);
            }
            i++;
        }
        else
        {
            // Token WORD
            char *word = extract_word(input, &i);
            if (word)
            {
                add_token(&tokens, TOKEN_WORD, word);
                free(word);
            }
            i++;
        }
    }

    return tokens;
}

// Funzione di test per stampare i token
void print_tokens(t_token *tokens)
{
    while (tokens != NULL) {
        printf("%d: ", tokens->type);
        printf("%s\n", tokens->value);
        tokens = tokens->next;
    }
}


