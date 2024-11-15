/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 12:04:11 by fabi              #+#    #+#             */
/*   Updated: 2024/11/15 12:26:58 by fabi             ###   ########.fr       */
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

// Espande una variabile d'ambiente
char *expand_env_var(const char *input, size_t *i, char **envp)
{
    size_t start;
    char *var_name;
    char *value;

    (*i)++;  // Salta il $
    start = *i;
    while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
        (*i)++;
    var_name = ft_strndup(&input[start], *i - start);
    value = get_env_value(var_name, envp);
    free(var_name);
    return (value ? value : ft_strdup(""));
}

// Gestisce i token heredoc
void handle_heredoc(t_token **tokens, const char *input, size_t *i)
{
    size_t start;

    add_token(tokens, TOKEN_HEREDOC, "<<");
    *i += 2;
    while (input[*i] && ft_isspace(input[*i]))
        (*i)++;
    start = *i;
    while (input[*i] && !ft_isspace(input[*i]) && !is_special_char(input[*i]))
        (*i)++;
    if (start < *i)
    {
        char *limiter = ft_strndup(&input[start], *i - start);
        add_token(tokens, 9, limiter);  // TOKEN_HEREDOC_LIMITER
        free(limiter);
    }
}

// Gestisce i caratteri di redirezione
void handle_redirect(t_token **tokens, const char *input, size_t *i)
{
    if (input[*i] == '<' && input[*i + 1] == '<')
    {
        handle_heredoc(tokens, input, i);
    }
    else if (input[*i + 1] == input[*i])
    {
        add_token(tokens, TOKEN_APPEND, ">>");
        (*i) += 2;
    }
    else
    {
        add_token(tokens, input[*i] == '>' ? TOKEN_REDIRECT_OUT : TOKEN_REDIRECT_IN,
                 input[*i] == '>' ? ">" : "<");
        (*i)++;
    }
}

// Processa il contenuto tra virgolette
void process_quoted_content(t_token **tokens, const char *input, size_t *i, char **envp)
{
    char quote_type;
    size_t start;
    char *content;
    char *result;
    char *temp;

    quote_type = input[*i];
    (*i)++;  // Salta la virgoletta iniziale
    result = ft_strdup("");
    while (input[*i] && input[*i] != quote_type)
    {
        if (input[*i] == '$' && quote_type == '"')
        {
            temp = expand_env_var(input, i, envp);
            content = ft_strjoin(result, temp);
            free(result);
            free(temp);
            result = content;
        }
        else
        {
            temp = ft_strndup(&input[*i], 1);
            content = ft_strjoin(result, temp);
            free(result);
            free(temp);
            result = content;
            (*i)++;
        }
    }
    if (!input[*i])
        free_exit_str(NULL, 1, "Error: unmatched quote");
    (*i)++;  // Salta la virgoletta finale
    add_token(tokens, TOKEN_TEXT, result);
    free(result);
}

// Processa una parola non quotata
void process_word(t_token **tokens, const char *input, size_t *i)
{
    size_t start;

    start = *i;
    while (input[*i] && !ft_isspace(input[*i]) && !is_special_char(input[*i]) && !is_quote(input[*i]))
        (*i)++;
    if (*i > start)
    {
        char *word = ft_strndup(&input[start], *i - start);
        add_token(tokens, TOKEN_WORD, word);
        free(word);
    }
}

// Lexer principale modificato
t_token *lexer(const char *input, size_t i, t_token *tokens, char **envp)
{
    while (input[i])
    {
        if (ft_isspace(input[i]))
            i++;
        else if (is_quote(input[i]))
            process_quoted_content(&tokens, input, &i, envp);
        else if (input[i] == '|')
        {
            add_token(&tokens, TOKEN_PIPE, "|");
            i++;
        }
        else if (is_redirect_char(input[i]))
            handle_redirect(&tokens, input, &i);
        else
            process_word(&tokens, input, &i);
    }
    return (tokens);
}
