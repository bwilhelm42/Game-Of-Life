EXEC = grid
DEBUG = debug
SRC_BASENAME = allocate_grid draw_grid free_2d_array get_run_mode run_simulation init_world scene_update
CU_SRC_BASENAME =  run_simulation_cuda main
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

profile:
	~/nsight-systems-2021.2.1/bin/nsys profile ./$(EXEC)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(EXEC) $(DEBUG)

re: fclean all