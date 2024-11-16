/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 20:15:11 by fabi              #+#    #+#             */
/*   Updated: 2024/11/16 17:58:17 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
* Unisce due stringhe e libera entrambe
* Ritorna la nuova stringa allocata
*/
char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*result;

	if (!s1 || !s2)
	{
		if (s1)
			free(s1);
		if (s2)
			free(s2);
		return (NULL);
	}
	result = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (result);
}

/*
* Cerca il valore di una variabile d'ambiente
* Ritorna una nuova stringa con il valore o NULL se non trovata
*/
char	*get_env_value(const char *var, char **envp)
{
	size_t	len;
	int		i;

	if (!var || !envp)
		return (NULL);
	len = ft_strlen(var);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, len) == 0 && envp[i][len] == '=')
			return (ft_strdup(envp[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}
