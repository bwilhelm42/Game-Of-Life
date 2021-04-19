EXEC = grid
DEBUG = debug
SRC_BASENAME = main init_world run_simulation allocate_grid draw_grid free_2d_array
SRCS = $(addsuffix .c, $(SRC_BASENAME))
OBJS = $(addsuffix .o, $(notdir $(SRC_BASENAME)))
LIBS = -lSDL2
INCLUDES = SDL2
CFLAGS += -Wall -Wextra -Werror -fsanitize=address
CC = gcc

.PHONY: clean fclean re all

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -I $(INCLUDES) -o $@ $^ $(LIBS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(EXEC) $(DEBUG)

re: fclean all