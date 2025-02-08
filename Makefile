NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -Iinclude
SRCS = libft/ft_putstr_fd.c src/main.c src/validate.c src/init.c src/utilities.c
OBJS = ${SRCS:.c=.o}
RM = rm -f

RED = \033[0;31m
GREEN = \033[0;32m
NC = \033[0m 

all: ${NAME}

${NAME}: ${OBJS}
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "\n$(GREEN)Compiling $(NAME) is ready to use!\n$(RED)USAGE:$(NC)./philo [number_of_philosophers] [time_to_die] [time_to_eat]\
	 [time_to_sleep] [number_of_times_each_philosopher_must_eat]\n"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)

fclean:
	@$(RM) $(OBJS)
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
