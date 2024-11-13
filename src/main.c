/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:11:18 by fabi              #+#    #+#             */
/*   Updated: 2024/11/13 20:05:45 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Funzione di test per stampare i token
void print_tokens(t_token *tokens)
{
	while (tokens != NULL)
	{
		printf("%d: ", tokens->type);
		printf("%s\n", tokens->value);
		tokens = tokens->next;
	}
}

int main(int argc, char **argv, char **envp)
{
	const char *input = "echo -n   \"hello world\" | grep 'pattern' > file.txt";
	//const char *input = "echo -n   \"       hello\" | cat                    > file.txt";
	//const char *input = "cat << EOF | grep \"pattern\" > output.txt && echo \"Done\"";
	t_token *tokens = lexer(input, 0, NULL);
	print_tokens(tokens);
	return (0);
}

