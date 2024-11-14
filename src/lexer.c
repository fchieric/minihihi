/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 21:53:46 by fabi              #+#    #+#             */
/*   Updated: 2024/11/14 19:28:08 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Dichiarazioni delle funzioni
char *extract_full_quote(const char *input, size_t *i, char **envp);
char *extract_word(const char *input, size_t *i);
void token_helper(t_token **tokens, const char *input, size_t *i);
char *get_env_value(const char *var, char **envp);
char *expand_variable(const char *input, size_t *start, size_t *end, char **envp);

char *expand_variable(const char *input, size_t *start, size_t *end, char **envp) {
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


char *get_env_value(const char *var, char **envp) {
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

// Funzione per estrarre il contenuto all'interno delle virgolette come un token unico
char *extract_full_quote(const char *input, size_t *i, char **envp) {
    size_t start = *i + 1; // Salta la virgoletta iniziale
    size_t end = start;
    char quote_char = input[*i]; // Salva il tipo di virgoletta (singola o doppia)
    char *text = ft_strdup(""); // Inizializza il testo come stringa vuota
    char *temp;

    printf("DEBUG: Entrato in extract_full_quote, i: %zu, start: %zu\n", *i, start);

    while (input[end]) {
        if (input[end] == quote_char) {
            // Trovata la chiusura della virgoletta
            temp = ft_strjoin_and_free(text, ft_strndup(&input[start], end - start));
            if (!temp)
                free_exit_str(NULL, 1, "Error: memory allocation failed");
            text = temp;

            *i = end + 1; // Avanza l'indice oltre la chiusura
            printf("DEBUG: Chiusura trovata, i: %zu, testo finale: '%s'\n", *i, text);
            return text;
        } else if (input[end] == '$' && quote_char == '"') {
            // Espansione variabile solo all'interno di doppi apici
            temp = ft_strjoin_and_free(text, ft_strndup(&input[start], end - start));
            if (!temp)
                free_exit_str(NULL, 1, "Error: memory allocation failed");
            text = temp;

            // Espandi la variabile
            end++; // Salta il simbolo $
            size_t var_start = end;
            while (input[end] && (ft_isalnum(input[end]) || input[end] == '_'))
                end++;

            char *env_name = ft_strndup(&input[var_start], end - var_start);
            char *env_value = get_env_value(env_name, envp);
            free(env_name);

            if (!env_value)
                env_value = ft_strdup(""); // Se la variabile non esiste, usa una stringa vuota

            temp = ft_strjoin_and_free(text, env_value);
            if (!temp)
                free_exit_str(NULL, 1, "Error: memory allocation failed");
            text = temp;

            start = end; // Aggiorna il punto di partenza dopo l'espansione
        } else {
            end++;
        }
    }

    // Se usciamo dal ciclo senza trovare la chiusura della virgoletta, errore
    free(text);
    free_exit_str(NULL, 1, "Error: unmatched quote");
    return NULL; // Non raggiungibile, ma per sicurezza
}

// Modifica la funzione principale del lexer per usare extract_full_quote
t_token *lexer(const char *input, size_t i, t_token *tokens, char **envp) {
    t_token_type type;
    char *token;
    t_token *last_token = NULL;

    while (input[i] != '\0') {
        printf("DEBUG: Inizio loop lexer, i: %zu, char: '%c'\n", i, input[i]);

        if (ft_isspace(input[i])) {
            printf("DEBUG: Spazio rilevato, i: %zu\n", i);
            i++;
        } else if (input[i] == '|') {
            add_token(&tokens, TOKEN_PIPE, "|");
            i++;
        } else if (input[i] == '>' || input[i] == '<') {
            token_helper(&tokens, input, &i);
        } else if (input[i] == '"' || input[i] == '\'') {
            // Riconosci una citazione aperta
            token = extract_full_quote(input, &i, envp);
            if (last_token && last_token->type == TOKEN_TEXT) {
                // Se l'ultimo token è TOKEN_TEXT, concatena il nuovo testo
                char *merged = ft_strjoin_and_free(last_token->value, token);
                if (!merged)
                    free_exit_str(NULL, 1, "Error: memory allocation failed");
                last_token->value = merged;
                free(token);
            } else {
                type = TOKEN_TEXT;
                add_token(&tokens, type, token);
                last_token = tokens;
                t_token *temp_token = tokens;
                while (temp_token && temp_token->next)
                    temp_token = temp_token->next;
                last_token = temp_token;
                free(token);
            }
        } else {
            size_t start = i;
            while (input[i] && !ft_isspace(input[i]) && input[i] != '|' && input[i] != '>' && input[i] != '<' && input[i] != '"' && input[i] != '\'') {
                if (input[i] == '$') {
                    // Espandi la variabile fuori dalle virgolette
                    char *temp_text = ft_strndup(&input[start], i - start);
                    size_t var_end = i;
                    char *expanded_var = expand_variable(input, &i, &var_end, envp);

                    // Unisci il testo prima della variabile con la variabile espansa
                    char *combined_text = ft_strjoin(temp_text, expanded_var);
                    free(temp_text);
                    free(expanded_var);

                    token = combined_text;
                    type = TOKEN_TEXT;

                    if (last_token && last_token->type == TOKEN_TEXT) {
                        // Se l'ultimo token è TOKEN_TEXT, concatena il nuovo testo
                        char *merged = ft_strjoin_and_free(last_token->value, token);
                        if (!merged)
                            free_exit_str(NULL, 1, "Error: memory allocation failed");
                        last_token->value = merged;
                        free(token);
                    } else {
                        add_token(&tokens, type, token);
                        last_token = tokens;
                        t_token *temp_token = tokens;
                        while (temp_token && temp_token->next)
                            temp_token = temp_token->next;
                        last_token = temp_token;
                        free(token);
                    }

                    start = var_end; // Aggiorna l'indice start alla fine dell'espansione della variabile
                    i = var_end;  // Avanza `i` per non rimanere bloccato
                } else {
                    i++;
                }
            }
            if (i > start) {
                token = ft_strndup(&input[start], i - start);
                if (last_token && last_token->type == TOKEN_TEXT) {
                    // Se l'ultimo token è TOKEN_TEXT, concatena il nuovo testo
                    char *merged = ft_strjoin_and_free(last_token->value, token);
                    if (!merged)
                        free_exit_str(NULL, 1, "Error: memory allocation failed");
                    last_token->value = merged;
                    free(token);
                } else {
                    type = TOKEN_WORD;
                    add_token(&tokens, type, token);
                    last_token = tokens;
                    t_token *temp_token = tokens;
                    while (temp_token && temp_token->next)
                        temp_token = temp_token->next;
                    last_token = temp_token;
                    free(token);
                }
            }
        }
    }

    printf("DEBUG: Fine lexer, tutti i token estratti.\n");
    return tokens;
}




// Funzione per estrarre una parola dall'input
char *extract_word(const char *input, size_t *i) {
    size_t start;
    size_t end;
    char *word;

    start = *i;
    while (input[start] && ft_isspace(input[start]))
        start++;
    end = start;
    while (input[end] && !ft_isspace(input[end]) && input[end] != '|' &&
           input[end] != '>' && input[end] != '<')
        end++;

    word = safe_malloc(end - start + 1);
    ft_strncpy(word, &input[start], end - start);
    word[end - start] = '\0';

    *i = end;
    return word;
}

/// Funzione per aggiungere un token alla lista
void token_helper(t_token **tokens, const char *input, size_t *i) {
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
