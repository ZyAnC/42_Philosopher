NAME = philo

CC := cc
CFLAGS := -Wall -Wextra -Werror\




SRCS :=philosophers.c tools.c check.c philo_running.c tools2.c
OBJS := ${SRCS:.c=.o}



all: $(NAME)

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $<

$(NAME): $(OBJS)

	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)



clean:
	@rm -rf $(OBJS)


fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
