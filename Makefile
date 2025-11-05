SRCS = src/main.c\
	src/parser.c\
	src/init.c\
	src/cleaning.c\
	src/process.c

#OBJS := $(SRCS:.c=.o)

OBJS_DIR = obj

OBJS = $(SRCS:src/%.c=$(OBJS_DIR)/%.o)

NAME = philosophers

HEADER = includes/philo.h

CC = cc

CFLAGS = -Wall -Wextra -Werror -g3

#LIBFT = libft/libft.a


all: $(NAME)

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJS_DIR)/%.o: src/%.c $(HEADER) | $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

#$(LIBFT):
#	make -C libft

bonus: $(NAME)

clean:
#	@make clean -C libft
	rm -rf $(OBJS_DIR)

fclean: clean
#	@make fclean -C libft
	rm -f $(NAME)

re: fclean all