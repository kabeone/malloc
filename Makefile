NAME = memlib.a
SRC  = src/malloc_function.c \
       src/pages.c
OBJ  = $(SRC:.c=.o)

RM   = rm -rf

all:		$(NAME)

$(NAME):		$(OBJ)
		ar crs $(NAME) $(OBJ)

clean:
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all
