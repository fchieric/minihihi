NAME = mini

SRC = main.c error.c lexer.c libft.c safe_funcs.c utils.c expander.c
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -g $(INCLUDES)
#tolgo le flag per ora: -Wall -Wextra -Werror

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

all: $(NAME)

.PHONY: clean fclean re all