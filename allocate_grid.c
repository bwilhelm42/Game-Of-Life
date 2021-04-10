#include "sdl_grid.h"

int**	allocate_grid() {
	int **temp;

	temp = (int**)malloc(sizeof(int*) * GRID_H);
	for (int i = 0; i < GRID_H; i++) {
		temp[i] = (int*)malloc(sizeof(int) * GRID_W);
		for (int j = 0; j < GRID_W; j++) {
			temp[i][j] = 0;
		}
	}
	return temp;
}