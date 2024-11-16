/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 21:30:11 by fabi              #+#    #+#             */
/*   Updated: 2024/11/16 18:52:25 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	test_lexer(const char *input, char **envp)
{
	t_token	*tokens;

	printf("\nInput: %s\n", input);
	tokens = lexer(input, envp);
	print_tokens(tokens);
	free_tokens(tokens);
}

void	print_tokens(t_token *tokens)
{
	static const char *type_str[] = {
		"WORD",
		"TEXT",
		"VAR",
		"PIPE",
		"REDIR_IN",
		"REDIR_OUT",
		"APPEND",
		"HEREDOC"
	};

	printf("Tokens:\n");
	while (tokens)
	{
		printf("Type: %s (%d), Value: %s\n", 
			type_str[tokens->type], tokens->type, tokens->value);
		tokens = tokens->next;
	}
	printf("-------------------\n");
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

	if (!envp)
		return (1);

	test_lexer("echo \"\"", envp);           // Una coppia di quote vuote
	test_lexer("echo \"\"\"\"", envp);       // Due coppie di quote vuote
	test_lexer("echo ''", envp);             // Quote singole vuote
	test_lexer("echo ''\"\"", envp);         // Mix di quote vuote
	test_lexer("echo \"\"''\"\"", envp);     // Multiple quote vuote miste
	
	// Test quote vuote con testo
	test_lexer("echo \"\"hello", envp);      // Quote vuote seguite da testo
	test_lexer("echo hello\"\"", envp);      // Testo seguito da quote vuote
	test_lexer("echo \"\"$USER\"\"", envp);  // Quote vuote con variabile

	return (0);
}