#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>


typedef enum e_token_type {
    TOKEN_WORD, // queste devono essere divise in comandi e argomenti e flag
    TOKEN_PIPE,
	TOKEN_END,
	TOKEN_QUOTE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_APPEND,
    TOKEN_HEREDOC,
} t_token_type;

typedef struct s_token {
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

typedef struct s_command {
    char **args;            // Argomenti del comando
    char *input_file;       // File di input
    char *output_file;      // File di output
    int append;             // Flag per ">>"
    struct s_command *next; // Supporto per pipe
} t_command;

t_token *lexer(const char *input);
void print_tokens(t_token *tokens);

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