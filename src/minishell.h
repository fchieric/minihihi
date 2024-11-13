/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:30:43 by fabi              #+#    #+#             */
/*   Updated: 2024/11/13 20:05:54 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <ctype.h>

// per stamparlo devo fare itoa di g_status 
//e devo vedere se g_status è compreso tra 0 e 255 perchè 256 diventa 0
// (if g_status >= 256) g_status = g_status - 255;
// posso farlo direttamente in echo $?
//int	g_status = 0;

// aggiungere anche gestione del $
// TOKEN_WORD queste devono essere divise in comandi e argomenti 
//e flag e EOF se li dobbiamo gestire e anche && e || che non dobbiamo gestire
// TOKEN_TEXT questo è quello tra le virgolette
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_END,
	TOKEN_QUOTE,
	TOKEN_TEXT,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}	t_token;

// typedef struct s_command
// {
// 	char **args;            // Argomenti del comando
// 	char *input_file;       // File di input
// 	char *output_file;      // File di output
// 	int append;             // Flag per ">>"
// 	struct s_command *next; // Supporto per pipe
// } t_command;

t_token	*lexer(const char *input, size_t i, t_token *tokens);
void	add_token(t_token **tokens, t_token_type type, const char *value);

/*
typedef struct s_var
{
	t_list				*env;
	char				*home_path;

	// -------------PWD-------------
	char				*curr_path;
	uint64_t			curpath_len;
	// -------------INPUT-----------
	t_list				*history;
	// -------------EXECUTION-------
	t_list				*all_cmds;
	int					**pipes;
	int					orig_stdin;
	int					orig_stdout;
	//------------------------------
	char				**script_file;

	t_uchar				statuscode;

	t_main				_main;
}			t_var;
*/

#endif