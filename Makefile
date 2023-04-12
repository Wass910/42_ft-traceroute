SRC=    srcs/ft_traceroute.c srcs/utils.c\

OBJS			= $(SRC:.c=.o)

NAME			= ft_traceroute

CFLAGS			= -Wall -Wextra -Werror 

RM				= rm -f

CC				= gcc

%.o : %.cpp
				$(CC) $(CFLAGS) -c $< -o $@

$(NAME):		$(OBJS)
				$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L.

all:			$(NAME)

clean:
				$(RM) $(OBJS) 

fclean:			clean
				$(RM) $(NAME)

re:				fclean all

.PHONY:			all clean fclean c.o re 