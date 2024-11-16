/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 18:30:43 by fabi              #+#    #+#             */
/*   Updated: 2024/11/16 18:26:38 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* -------- INCLUDES -------- */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <ctype.h>

/* -------- GLOBAL VARIABLE -------- */
/* 
*  Verrà utilizzata solo per gestire i segnali come richiesto dal subject:
*  "Avoid using more than one global variable to indicate a received signal"
*/
extern int	g_status;

/* -------- TOKEN TYPES -------- */
/*
*  Ogni tipo di token che il lexer può identificare:
*  - WORD: parole normali (comandi, argomenti)
*  - TEXT: contenuto tra virgolette (singole o doppie)
*  - VAR: variabili d'ambiente ($VAR)
*  - PIPE: carattere pipe (|)
*  - REDIR_IN/OUT: redirezioni (< >)
*  - APPEND: redirect con append (>>)
*  - HEREDOC: heredoc (<<)
*  - EOF: fine del file/input
*/
typedef enum e_token_type
{
    TOKEN_WORD = 0,     // comandi e argomenti normali
    TOKEN_TEXT = 1,     // contenuto tra quote (singole o doppie)
    TOKEN_VAR = 2,      // variabili d'ambiente espanse
    TOKEN_PIPE = 3,     // |
    TOKEN_REDIR_IN = 4, // <
    TOKEN_REDIR_OUT = 5,// >
    TOKEN_APPEND = 6,   // >>
    TOKEN_HEREDOC = 7   // <<
}   t_token_type;

/* -------- STRUCTURES -------- */

/*
*  Struttura per i token:
*  - type: tipo del token (dalla enum e_token_type)
*  - value: contenuto del token
*  - next: puntatore al prossimo token
*/
typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;

/* -------- LEXER FUNCTIONS -------- */
t_token     *lexer(const char *input, char **envp);
char        *get_env_value(const char *var, char **envp);

/* -------- UTILS FUNCTIONS -------- */
void        add_token(t_token **tokens, t_token_type type, const char *value);
void        free_token(t_token *token);
void        free_tokens(t_token *tokens);
void        print_tokens(t_token *tokens); // per debug

/* -------- LIBFT FUNCTIONS -------- */
int         ft_isspace(int c);
size_t      ft_strlen(const char *s);
char        *ft_strdup(const char *s1);
char        *ft_substr(const char *s, unsigned int start, size_t len);
char        *ft_strjoin(char const *s1, char const *s2);
char        *ft_strjoin_free(char *s1, char *s2);
int         ft_strncmp(const char *s1, const char *s2, size_t n);
void        ft_bzero(void *s, size_t n);
char        *ft_strchr(const char *s, int c);
int         ft_isalnum(int c);

/* -------- ERROR HANDLING -------- */
void        *safe_malloc(size_t size);
void        error_exit(char *message);


char	*get_env_value(const char *var, char **envp);
char	*ft_strjoin_free(char *s1, char *s2);
#endif