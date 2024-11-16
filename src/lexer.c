/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 20:45:11 by fabi              #+#    #+#             */
/*   Updated: 2024/11/16 18:51:45 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Forward declarations */
static int		is_special_char(char c);
static char		*get_word(const char *input, size_t *i);
static char		*expand_env_var(const char *input, size_t *i, char **envp);
static char		*handle_single_quotes(const char *input, size_t *i);
static char		*handle_double_quotes(const char *input, size_t *i, char **envp);
static void		handle_quotes(t_token **tokens, const char *input, size_t *i, 
					char **envp);
static void		handle_special_chars(t_token **tokens, const char *input, 
					size_t *i);

static int	is_special_char(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

static char	*get_word(const char *input, size_t *i)
{
	size_t	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_special_char(input[*i])
		&& input[*i] != '\'' && input[*i] != '\"' && input[*i] != '$')
		(*i)++;
	return (ft_substr(input, start, *i - start));
}

static char	*expand_env_var(const char *input, size_t *i, char **envp)
{
	size_t	start;
	char	*var_name;
	char	*value;

	(*i)++;
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_name = ft_substr(input, start, *i - start);
	if (!var_name)
		return (ft_strdup(""));
	value = get_env_value(var_name, envp);
	free(var_name);
	return (value ? value : ft_strdup(""));
}

static char	*handle_single_quotes(const char *input, size_t *i)
{
	size_t	start;
	char	*content;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (!input[*i])  // Quote non chiuse
		return (NULL);
	if (*i == start)  // Quote vuote
		content = ft_strdup("");
	else
		content = ft_substr(input, start, *i - start);
	(*i)++;
	return (content);
}


static char	*handle_double_quotes(const char *input, size_t *i, char **envp)
{
	char	*result;
	char	*tmp;
	size_t	start;

	result = ft_strdup("");
	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$' && input[*i + 1] && 
			(ft_isalnum(input[*i + 1]) || input[*i + 1] == '_'))
		{
			tmp = ft_substr(input, start, *i - start);
			result = ft_strjoin_free(result, tmp);
			tmp = expand_env_var(input, i, envp);
			result = ft_strjoin_free(result, tmp);
			start = *i;
		}
		else
			(*i)++;
	}
	if (!input[*i])  // Quote non chiuse
		return (result);
	if (*i > start)
	{
		tmp = ft_substr(input, start, *i - start);
		result = ft_strjoin_free(result, tmp);
	}
	(*i)++;
	return (result);
}


static void	handle_special_chars(t_token **tokens, const char *input, size_t *i)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		add_token(tokens, TOKEN_HEREDOC, "<<");
		*i += 2;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		add_token(tokens, TOKEN_APPEND, ">>");
		*i += 2;
	}
	else if (input[*i] == '>')
	{
		add_token(tokens, TOKEN_REDIR_OUT, ">");
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		add_token(tokens, TOKEN_REDIR_IN, "<");
		(*i)++;
	}
	else if (input[*i] == '|')
	{
		add_token(tokens, TOKEN_PIPE, "|");
		(*i)++;
	}
}

static void	handle_quotes(t_token **tokens, const char *input, size_t *i, 
				char **envp)
{
	char	quote_type;
	char	*content;
	char	*tmp;
	char	*result;
	int		empty_quote;

	result = ft_strdup("");
	empty_quote = 1;  // Flag per tenere traccia se stiamo processando solo quote vuote
	while (input[*i] && (input[*i] == '\'' || input[*i] == '"' || 
		input[*i] == '$'))
	{
		if (input[*i] == '\'')
			content = handle_single_quotes(input, i);
		else if (input[*i] == '"')
			content = handle_double_quotes(input, i, envp);
		else
		{
			content = expand_env_var(input, i, envp);
			if (content && *content)
				empty_quote = 0;
		}
		if (!content)
			content = ft_strdup("");
		if (*content)
			empty_quote = 0;
		tmp = result;
		result = ft_strjoin(tmp, content);
		free(tmp);
		free(content);
	}
	// Aggiungi il token anche se è vuoto
	add_token(tokens, TOKEN_TEXT, result);
	free(result);
}


t_token	*lexer(const char *input, char **envp)
{
	t_token	*tokens;
	size_t	i;
	char	*word;

	tokens = NULL;
	i = 0;
	while (input && input[i])
	{
		if (ft_isspace(input[i]))
			i++;
		else if (input[i] == '\'' || input[i] == '"' || input[i] == '$')
			handle_quotes(&tokens, input, &i, envp);
		else if (is_special_char(input[i]))
			handle_special_chars(&tokens, input, &i);
		else
		{
			word = get_word(input, &i);
			if (word && *word)
				add_token(&tokens, TOKEN_WORD, word);  // Corretto: passiamo &tokens
			free(word);
		}
	}
	return (tokens);
}
