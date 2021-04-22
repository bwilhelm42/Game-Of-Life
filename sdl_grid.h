#ifndef SDL_GRID_H
#define SDL_GRID_H

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define CELL_SIZE 26
#define GRID_H 45
#define GRID_W 90
#define WINDOW_H (GRID_H * CELL_SIZE) + 1
#define WINDOW_W (GRID_W * CELL_SIZE) + 1

#define BILLION 1000000000.0

#define GPU 0
#define CPU 1
#define MODE GPU

int*    init_world(SDL_Renderer *renderer);
int     get_run_mode();
void    run_simulation(int** scene, SDL_Renderer *renderer, int delay);
int**	allocate_grid();
void	draw_grid(SDL_Renderer *renderer);
void    free_2d_array(int x, int** array);

#endif