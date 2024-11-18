/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 21:30:11 by fabi              #+#    #+#             */
/*   Updated: 2024/11/18 12:25:59 by fabi             ###   ########.fr       */
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

	test_lexer("echo \"$USER$HOME\"", envp);
	test_lexer("echo \"\'$USER\'\"", envp);
	test_lexer("echo \"$NONEXISTENT\"", envp);
	test_lexer("echo \"hello$USER\"world", envp);

	return (0);
}



