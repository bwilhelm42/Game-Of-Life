#include "sdl_grid.h"

void    free_2d_array(int x, int** array) {
    while (--x >= 0) {
        free(array[x]);
    }
    free(array);
}