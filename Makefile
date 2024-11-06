NAME = philo
CC = cc

LINKFLAGS = -pthread

CFLAGS =	-Wall -Wextra -Werror \
			-g3 -gdwarf-3 \
			-O0 \
			-pthread \
			-fsanitize=thread \

SRC_DIR = ./
OBJ_DIR = ./obj/

SRC =	philosophers.c \
		input_val_init.c \
		utils.c \
		philo_actions.c \
		exit_cleanup.c \
		philo_routine.c \
		judge_routine.c \

SRCS = $(addprefix $(SRC_DIR), $(SRC))
OBJ = $(SRC:.c=.o)
OBJS = $(addprefix $(OBJ_DIR), $(OBJ))

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(LINKFLAGS)

all: $(NAME)

clean:
	@rm -rf $(OBJS)
	@rmdir $(OBJ_DIR) || true

fclean: clean
	@rm -f $(NAME)

re: fclean all

valgrind: re
	valgrind \
	--trace-children=yes \
	--leak-check=full \
	./$(NAME) \
	10 10 10 10

.PHONY: all clean fclean re valgrind