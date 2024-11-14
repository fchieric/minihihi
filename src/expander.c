/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:47:38 by fabi              #+#    #+#             */
/*   Updated: 2024/11/14 18:57:23 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"




char *expand_env_in_quotes(const char *input, size_t *start, size_t *end, char **envp, char *text)
{
    char *env_name;
    char *env_value;
    char *temp;

    (*end)++; // Salta il simbolo `$`
    *start = *end;
    while (input[*end] && (ft_isalnum(input[*end]) || input[*end] == '_'))
        (*end)++;

    env_name = ft_strndup(&input[*start], *end - *start);
    printf("DEBUG: Espansione variabile trovata: '%s'\n", env_name);
    env_value = get_env_value(env_name, envp);
    free(env_name);

    if (!env_value)
        env_value = ft_strdup(""); // Variabile non trovata, usa stringa vuota

    printf("DEBUG: Valore espanso: '%s'\n", env_value);
    temp = ft_strjoin_and_free(text, env_value); // Concatena e libera `text`
    free(env_value);

    *start = *end; // Aggiorna il punto di partenza
    return temp;   // Restituisce la nuova stringa concatenata
}
