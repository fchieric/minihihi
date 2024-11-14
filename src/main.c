/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:11:18 by fabi              #+#    #+#             */
/*   Updated: 2024/11/14 13:54:50 by fabi             ###   ########.fr       */
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
	//const char *input = "echo -n   \"hello world\" | grep 'pattern' > file.txt";
	//const char *input = "echo -n   \"       hello\" | cat                    > file.txt";
	//const char *input = "cat << EOF | grep \"pattern\" > output.txt && echo \"Done\"";
	//const char *input = "echo \"hello 'world'\" | grep 'pattern >' >> output.txt";
	//const char *input = "echo \"hello world | grep 'pattern";
	//const char *input = "cat << EOF | echo \"$HOME\" > result.txt";
	const char *input = "echo \"hello $USER, welcome\"";
	if (!envp || !*envp) // Verifica se `envp` è NULL o vuoto
	{
		fprintf(stderr, "Error: Environment variables are not available\n");
		return (1);
	}
	t_token *tokens = lexer(input, 0, NULL, envp);
	print_tokens(tokens);
	return (0);
}

