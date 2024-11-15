/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 19:30:43 by fabi              #+#    #+#             */
/*   Updated: 2024/11/15 12:16:03 by fabi             ###   ########.fr       */
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
	TOKEN_END, // Fine dell'intero input
	TOKEN_QUOTE,
	TOKEN_TEXT,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF 
}	t_token_type;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

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

//lexer.c
t_token *lexer(const char *input, size_t i, t_token *tokens, char **envp);

//utils.c
void	add_token(t_token **tokens, t_token_type type, const char *value);

//safe_funcs.c
void	*safe_malloc(size_t size);

//error.c
void	free_exit_list(t_token *tokens, int exit_status, char *message);
void	free_exit_str(char *str, int exit_status, char *message);

//libft.c
int		ft_isspace(int c);
void	ft_strncpy(char *dst, const char *src, size_t n);
char	*ft_strdup(const char *s1);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strndup(const char *s1, size_t n);
int		ft_isalnum(int c);
int		ft_strlen(const char *s);
char	*ft_strjoin_and_free(char *s1, const char *s2);

//expander.c
char	*expand_env_in_quotes(const char *input, size_t *start, size_t *end, char **envp, char *text);
char	*get_env_value(const char *var, char **envp);

char	*expand_variable(const char *input, size_t *start, size_t *end, char **envp);
void	token_helper(t_token **tokens, const char *input, size_t *i);
char	*handle_variable(const char *input, size_t *i, size_t start, char **envp);


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