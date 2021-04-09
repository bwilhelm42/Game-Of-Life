#include "sdl_grid.h"

bool**	allocate_grid() {
	bool **temp;

	temp = (bool**)malloc(sizeof(bool*) * GRID_H);
	for (int i = 0; i < GRID_H; i++) {
		temp[i] = (bool*)malloc(sizeof(bool) * GRID_W);
		bzero(temp[i], GRID_W);
	}
	return temp;
}