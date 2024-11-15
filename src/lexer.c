/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:04:11 by fabi              #+#    #+#             */
/*   Updated: 2024/11/15 12:20:32 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_env_value(const char *var, char **envp)
{
    size_t len = ft_strlen(var);
    int i = 0;

    while (envp[i]) {
        if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=') {
            return ft_strdup(envp[i] + len + 1); // Restituisce il valore dopo il simbolo "="
        }
        i++;
    }

    return NULL; // Se la variabile non è trovata
}

void token_helper(t_token **tokens, const char *input, size_t *i)
{
    if (input[*i] == '>' || input[*i] == '<') {
        char type = input[*i];
        if (input[*i + 1] == type) {
            if (type == '>')
                add_token(tokens, TOKEN_APPEND, ">>");
            else
                add_token(tokens, TOKEN_HEREDOC, "<<");
            *i += 2;
        } else {
            if (type == '>')
                add_token(tokens, TOKEN_REDIRECT_OUT, ">");
            else
                add_token(tokens, TOKEN_REDIRECT_IN, "<");
            (*i)++;
        }
    }
}

char *expand_variable(const char *input, size_t *start, size_t *end, char **envp)
{
    (*end)++; // Salta il simbolo `$`
    size_t var_start = *end;
    
    // Trova la fine del nome della variabile
    while (input[*end] && (ft_isalnum(input[*end]) || input[*end] == '_')) {
        (*end)++;
    }

    // Estrai il nome della variabile
    char *var_name = ft_strndup(&input[var_start], *end - var_start);
    char *value = get_env_value(var_name, envp);
    free(var_name);

    if (!value) {
        value = ft_strdup(""); // Se la variabile non è trovata, restituisce stringa vuota
    }

    return value;
}

// Controlla se è un carattere di virgoletta
int is_quote(char c)
{
    return (c == '"' || c == '\'');
}

// Controlla se è un carattere speciale di redirezione
int is_redirect_char(char c)
{
    return (c == '>' || c == '<');
}

// Controlla se è un carattere speciale
int is_special_char(char c)
{
    return (c == '|' || is_redirect_char(c));
}

// Ottiene l'ultimo token
t_token *get_last_token(t_token *tokens)
{
    while (tokens && tokens->next)
        tokens = tokens->next;
    return (tokens);
}

// Aggiunge una singola parola come token
void add_word_token(t_token **tokens, const char *input, size_t start, size_t end)
{
    char *word;

    if (end <= start)
        return;
    word = ft_strndup(&input[start], end - start);
    add_token(tokens, TOKEN_WORD, word);
    free(word);
}

// Gestisce i caratteri di redirezione
void handle_redirect(t_token **tokens, const char *input, size_t *i)
{
    char current;
    
    current = input[*i];
    if (input[*i + 1] == current)
    {
        if (current == '>')
            add_token(tokens, TOKEN_APPEND, ">>");
        else
            add_token(tokens, TOKEN_HEREDOC, "<<");
        (*i) += 2;
    }
    else
    {
        if (current == '>')
            add_token(tokens, TOKEN_REDIRECT_OUT, ">");
        else
            add_token(tokens, TOKEN_REDIRECT_IN, "<");
        (*i)++;
    }
}

// Processa il contenuto tra virgolette
void process_quoted_content(t_token **tokens, const char *input, size_t *i, char **envp)
{
    char quote_type;
    size_t start;
    char *content;

    quote_type = input[*i];
    start = *i;
    (*i)++;
    while (input[*i] && input[*i] != quote_type)
        (*i)++;
    if (!input[*i])
        free_exit_str(NULL, 1, "Error: unmatched quote");
    (*i)++;
    content = ft_strndup(&input[start], *i - start);
    add_token(tokens, TOKEN_TEXT, content);
    free(content);
}

// Processa una parola non quotata
void process_word(t_token **tokens, const char *input, size_t *i)
{
    size_t start;

    start = *i;
    while (input[*i] && !ft_isspace(input[*i]) && !is_special_char(input[*i]) && !is_quote(input[*i]))
        (*i)++;
    add_word_token(tokens, input, start, *i);
}

// Lexer principale modificato
t_token *lexer(const char *input, size_t i, t_token *tokens, char **envp)
{
    size_t start;

    while (input[i])
    {
        if (ft_isspace(input[i]))
        {
            i++;
            continue;
        }
        if (is_quote(input[i]))
        {
            process_quoted_content(&tokens, input, &i, envp);
        }
        else if (input[i] == '|')
        {
            add_token(&tokens, TOKEN_PIPE, "|");
            i++;
        }
        else if (is_redirect_char(input[i]))
        {
            handle_redirect(&tokens, input, &i);
        }
        else
        {
            process_word(&tokens, input, &i);
        }
    }
    return (tokens);
}