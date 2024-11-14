/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:47:38 by fabi              #+#    #+#             */
/*   Updated: 2024/11/14 13:54:08 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(const char *var, char **envp)
{
	size_t	len;
	int		i;

	if (!envp) // Controlla se `envp` è NULL
		return (NULL);

	len = strlen(var);
	i = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
			return (strdup(envp[i] + len + 1)); // Restituisce il valore dopo "="
		i++;
	}
	return (NULL); // Variabile non trovata
}


char	*expand_env_in_quotes(const char *input, size_t *start, size_t *end, char **envp, char *text)
{
	char	*env_name;
	char	*env_value;
	char	*temp;

	(*end)++; // Salta il simbolo `$`
	*start = *end;
	while (input[*end] && (ft_isalnum(input[*end]) || input[*end] == '_'))
		(*end)++;
	env_name = ft_strndup(&input[*start], *end - *start);
	env_value = get_env_value(env_name, envp);
	free(env_name);
	if (!env_value)
		env_value = ft_strdup(""); // Variabile non trovata, usa stringa vuota
	temp = ft_strjoin(text, env_value);
	free(env_value);
	free(text); // Libera il vecchio `text` solo dopo la concatenazione
	*start = *end; // Aggiorna il punto di partenza
	return (temp);
}


