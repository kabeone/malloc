CC   = gcc -Wall -Wextra -std=c11
NAME = malloc
SRC  = malloc.c \
	   src/malloc_function.c
OBJ  = $(SRC:.c=.o)

RM   = rm -rf

all:		$(NAME)

$(NAME):    	$(OBJ)
			$(CC) $(OBJ) -o $(NAME)

clean:
			$(RM) $(OBJ)

fclean:		clean
			$(RM) $(NAME) $(NAME).dSYM

re:			fclean all

debug:		fclean
			rm -rf $(NAME).dSYM
			$(CC) $(SRC) -g -o $(NAME)
