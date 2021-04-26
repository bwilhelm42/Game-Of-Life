EXEC = grid
DEBUG = debug
SRC_BASENAME = main allocate_grid draw_grid free_2d_array get_run_mode
CU_SRC_BASENAME =  init_world run_simulation_cuda
SRCS = $(addsuffix .c, $(SRC_BASENAME))
CU_SRCS = $(addsuffix .cu, $(CU_SRC_BASENAME))
OBJS = $(addsuffix .o, $(notdir $(SRC_BASENAME))) $(addsuffix .o, $(notdir $(CU_SRC_BASENAME)))
LIBS = -lSDL2
INCLUDES = SDL2
FSANITIZE = -fsanitize=address
#CFLAGS += -Wall -Wextra -Werror
CC = nvcc

.PHONY: clean fclean re all

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -I $(INCLUDES) -o $@ $^ $(LIBS)

%.o: %.cu
	$(CC) -I $(INCLUDES) -c $< $(LIBS)

check : 
	$(info $$OBJS is [$(OBJS)])

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(EXEC) $(DEBUG)

re: fclean all