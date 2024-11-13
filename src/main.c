/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 15:11:18 by fabi              #+#    #+#             */
/*   Updated: 2024/11/13 17:58:22 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    const char *input = "echo -n   \"       hello\" | cat                    > file.txt";
    //const char *input = "cat << EOF | grep \"pattern\" > output.txt && echo \"Done\"";
    t_token *tokens = lexer(input, 0, NULL);
    print_tokens(tokens);
    return (0);
}

